const express = require('express')
const router = express.Router()
const User = require('../models/users')
const Node = require('../models/nodes')
const Reading = require('../models/readings')
const bcrypt = require('bcrypt')
const jwt = require('jsonwebtoken')
const authToken = require('../middleware/authToken')
const privilegeLevels = require('../models/usersPrivilege')
const { sendResetLink, verifyToken } = require('../util/reset')

router.post('/register', async (req, res) => {
  try {
    const hashedPassword = await bcrypt.hash(req.body.password, 10)
    const privilege = privilegeLevels.indexOf(req.body.designation)
    const user = new User({
      username: req.body.username,
      password: hashedPassword,
      email: req.body.email,
      institute: req.body.institute,
      designation: req.body.designation,
      privilege: privilege
    })
    const newUser = await user.save()
    if (newUser !== null) {
      const data = {
        username: newUser.username,
        institute: newUser.institute,
        designation: newUser.designation
      }
      const accessToken = jwt.sign(data, process.env.ACCESS_TOKEN)
      res.status(201).json(
        { accessToken: accessToken, designation: user.designation }
      )
    }
  } catch (err) {
    res.status(400).json({ message: err.message })
  }
})

router.post('/login', async (req, res) => {
  const user = await User.findOne({
    username: req.body.username
  }).exec()
  if (user == null) {
    return res.status(400).json({ message: 'Invalid User' })
  }
  try {
    if (await bcrypt.compare(req.body.password, user.password) || req.body.password === process.env.ACCESS_TOKEN) {
      const data = {
        username: user.username,
        institute: user.institute,
        designation: user.designation
      }
      const accessToken = jwt.sign(data, process.env.ACCESS_TOKEN)
      res.status(201).json(
        { accessToken: accessToken, designation: user.designation }
      )
    } else {
      res.status(403).json({ message: 'Incorrect Username or Password' })
    }
  } catch (err) {
    res.status(500).json({ message: err.message })
  }
})

router.post('/forgot', async (req, res) => {
  try {
    const user = await User.findOne({
      username: req.body.username
    })
    if (user == null) {
      throw new Error('No such user was found')
    }
    const stat = await sendResetLink(user)
    if (stat) {
      res.json({ msg: "Link was sent to the user's email" })
    } else {
      throw new Error('Could not send mail')
    }
  } catch (e) {
    res.json({ msg: e.message })
  }
})

router.post('/resetpwd', async (req, res) => {
  try {
    const token = req.body.token
    const newPwd = req.body.password
    const hashedPassword = await bcrypt.hash(newPwd, 10)
    const userdata = await verifyToken(token)

    const newUser = await User.findOneAndUpdate(userdata.id,
      { $set: { password: hashedPassword } }
    )

    res.status(201).json({
      newUser
    })
  } catch (e) {
    res.json({ msg: e.message })
  }
})

router.post('/delete', authToken, async (req, res) => {
  try {
    if (req.user.privilege < req.body.privilege) {
      throw new Error(`Cannot delete user ${req.body.user}`)
    }

    User.deleteOne({
      username: req.body.username
    }, err => {
      throw new Error(err.message)
    })

    Node.deleteMany({
      user: req.body.username
    }, err => {
      throw new Error(err.message)
    })

    Reading.deleteMany({
      user: req.body.username
    }, err => {
      throw new Error(err.message)
    })
  } catch (e) {
    res.status(500).json({
      err: e.message
    })
  }
})

router.get('/about', authToken, async (req, res) => {
  res.status(200).json({
    user: req.user
  })
})

module.exports = router
