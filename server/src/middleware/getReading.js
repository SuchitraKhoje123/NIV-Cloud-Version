const Reading = require('../models/readings')

async function getReading (req, _res, next) {
  let reading
  try {
    reading = await Reading.find({
      uid: req.params.uid
    }).sort({
      datetime: -1
    })
    req.reading = reading[0]
  } catch (e) {
    req.err = e.message
  }
  next()
}

module.exports = getReading
