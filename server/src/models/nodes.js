const mongoose = require('mongoose')

const RangeSchema = new mongoose.Schema({
  min: {
    type: Number
  },
  max: {
    type: Number
  }
})

const nodeSchema = new mongoose.Schema({
  uid: {
    type: String,
    required: true,
    unique: true
  },
  location: {
    type: String,
    required: true
  },
  machineName: {
    type: String,
    required: true
  },
  isTemperature: {
    type: Boolean,
    required: true
  },
  isHumidity: {
    type: Boolean,
    required: true
  },
  isCO2: {
    type: Boolean,
    required: true
  },
  co2Range: {
    type: RangeSchema,
    default: {
      min: 0,
      max: 100
    }
  },
  pressureRange: {
    type: RangeSchema,
    default: {
      min: 0,
      max: 100
    }
  },
  temperatureRange: {
    type: RangeSchema,
    default: {
      min: 0,
      max: 100
    }
  },
  humidityRange: {
    type: RangeSchema,
    default: {
      min: 0,
      max: 100
    },
    required: true
  },
  user: {
    type: String,
    required: true
  }
})

module.exports = mongoose.model('node', nodeSchema)
