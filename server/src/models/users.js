const mongoose = require('mongoose')

const userSchema = new mongoose.Schema({
  username: {
    type: String,
    required: true,
    unique: true
  },
  email: {
    type: String,
    required: true
  },
  designation: {
    type: String,
    required: true
  },
  privilege: {
    type: Number,
    required: true
  },
  institute: {
    type: String,
    required: true
  },
  password: {
    type: String,
    required: true
  },
  mailSent: {
    type: Date
  },
  faulty: [{
    type: String
  }]
})

module.exports = mongoose.model('user', userSchema)
