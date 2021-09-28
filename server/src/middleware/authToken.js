require('dotenv').config()
const jwt = require('jsonwebtoken')
const User = require('../models/users')

async function authenticateToken (req, res, next) {
  const authHeader = req.headers.authorization
  const token = authHeader && authHeader.split(' ')[1]
  if (token == null) return res.sendStatus(401)
  let usr

  try {
    jwt.verify(token, process.env.ACCESS_TOKEN, (err, user) => {
      if (err) throw new Error('Invalid User')
      usr = user
    })

    const userPresent = await User.findOne({
      username: usr.username
    })

    if (userPresent == null) {
      return res.sendStatus(403)
    }

    req.user = userPresent
    next()
  } catch (e) {
    res.status(403).json({ msg: e.message })
  }
}

module.exports = authenticateToken
