const express = require('express')
const router = express.Router()
const Node = require('../models/nodes')
const Reading = require('../models/readings')

const authenticateToken = require('../middleware/authToken')
const getNode = require('../middleware/getNode')
const createCSV = require('../util/createCSV')

router.get('/', authenticateToken, async (req, res) => {
  try {
    let nodes
    if (req.user.privilege === 1 || req.user.privilege === 3) {
      nodes = await Node.find({ location: req.user.institute })
    } else {
      nodes = await Node.find({ user: req.user.username })
    }
    res.json(nodes)
  } catch (err) {
    res.json({ message: err.message })
  }
})

router.get('/readings/:uid', authenticateToken, async (req, res) => {
  const UID = req.params.uid
  let readings
  try {
    readings = await Reading.find({
      uid: UID
    }).sort({
      datetime: -1
    })
    delete readings[0].__v
    res.status(200).json(readings[0])
  } catch (err) {
    res.status(500).json({ message: err.message })
  }
})

router.get('/readings/all/:uid', authenticateToken, async (req, res) => {
  const UID = req.params.uid
  let readings
  try {
    readings = await Reading.find({
      uid: UID
    }).sort()
    res.status(200).json(readings)
  } catch (err) {
    res.status(500).json({ message: err.message })
  }
})

router.post('/add', authenticateToken, async (req, res) => {
  req.body.user = req.user.username
  const node = new Node({
    uid: req.body.uid,
    location: req.body.location,
    machineName: req.body.machineName,
    user: req.user.username,
    isTemperature: req.body.isTemp,
    isHumidity: req.body.isHum,
    isCO2: req.body.isCO2,
    temperatureRange: {
      min: req.body?.temperatureRange?.min,
      max: req.body?.temperatureRange?.max
    },
    humidityRange: {
      min: req.body?.humidityRange?.min,
      max: req.body?.humidityRange?.max
    },
    co2Range: {
      min: req.body?.co2Range?.min,
      max: req.body?.co2Range?.max
    }
  })
  const reading = new Reading({
    uid: req.body.uid,
    user: req.user.username
  })
  try {
    if (req.user.privilege > 2) {
      throw new Error('You are not allowed to change nodes')
    }
    const newNode = await node.save()
    const newReading = await reading.save()
    res.status(201).json({
      node: newNode,
      reading: newReading
    })
  } catch (err) {
    res.status(400).json({ message: err.message })
  }
})

router.post('/modify', authenticateToken, getNode, async (req, res) => {
  const conditions = { uid: req.node.uid }
  const update = { $set: req.body }

  try {
    const newNode = await Node.findOneAndUpdate(conditions, update)
    res.status(201).json({ node: newNode })
  } catch (err) {
    res.status(404).json({ message: err.message })
  }
})

router.delete('/:uid', authenticateToken, async (req, res) => {
  try {
    let tbd
    if (req.user.privilege > 2) {
      throw new Error('You are not allowed to change nodes')
    }
    if (req.user.privilege < 2) {
      tbd = await Node.findOne({
        uid: req.params.uid
      })
    } else {
      tbd = await Node.findOne({
        uid: req.params.uid,
        user: req.user.username
      })
    }
    if (tbd == null) {
      throw new Error('Nothing to delete')
    }
    Reading.deleteMany({ uid: tbd.uid }, (err) => {
      if (err) {
        throw new Error(err.message)
      }
    })
    Node.deleteOne({ uid: tbd.uid }, function (err) {
      if (err) {
        throw new Error(err.message)
      }
    })
    res.status(200).json({
      message: 'Deleted Successfully'
    })
  } catch (err) {
    res.status(503).json({ message: err.message })
  }
})

router.get('/getcsv/:uid', async (req, res) => {
  try {
    const uid = req.params.uid
    const readingsDB = await Reading.find({ uid: uid })
    const readingsToSend = []
    for (let i = 0; i < readingsDB.length; i++) {
      readingsToSend.push({
        uid: readingsDB[i].uid,
        user: readingsDB[i].user,
        datetime: readingsDB[i].datetime,
        pressure: readingsDB[i].pressure,
        humidity: readingsDB[i].humidity,
        co2: readingsDB[i].co2,
        temperature: readingsDB[i].temperature
      })
    }

    res.setHeader('Content-Type', 'text/csv')
    res.setHeader('Content-Disposition', `attachment; filename="data.${uid}.csv"`)
    await createCSV(readingsToSend, res)
  } catch (err) {
    res.json({ msg: err.message })
  }
})

router.get('/convert2csv', async (req, res) => {
  try {
    const uid = req.params.uid
    const readingsDB = await Reading.find({ uid: uid })
    const readingsToSend = []
    for (let i = 0; i < readingsDB.length; i++) {
      readingsToSend.push({
        uid: readingsDB[i].uid,
        user: readingsDB[i].user,
        datetime: readingsDB[i].datetime,
        pressure: readingsDB[i].pressure,
        humidity: readingsDB[i].humidity,
        co2: readingsDB[i].co2,
        temperature: readingsDB[i].temperature
      })
    }

    res.setHeader('Content-Type', 'text/csv')
    res.setHeader('Content-Disposition', `attachment; filename="data.${uid}.csv"`)
    await createCSV(readingsToSend, res)
  } catch (err) {
    res.json({ msg: err.message })
  }
})

module.exports = router
