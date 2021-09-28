const mongoose = require('mongoose')
const nodeSchema = new mongoose.Schema({
  uid: {
    type: String,
    required: true
  },
  user: {
    type: String,
    required: true
  },
  temperature: {
    type: String
  },
  humidity: {
    type: String
  },
  pressure: {
    type: String
  },
  co2: {
    type: String
  },
  battery: {
    type: String
  },
  datetime: {
    type: Date,
    required: true,
    default: Date.now
  }
})
module.exports = mongoose.model('reading', nodeSchema)
