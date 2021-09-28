const User = require('../models/users')
const Node = require('../models/nodes')
const nodemailer = require('nodemailer')

const FiveHrs = 5 * 60 * 60 * 1000
const transporter = nodemailer.createTransport({
  service: 'gmail',
  auth: {
    user: process.env.FROM_EMAIL,
    pass: process.env.FROM_PWD
  }
})

const reportMail = async (uid) => {
  const node = await Node.findOne({ uid: uid })
  const username = node.user
  const user = await User.findOne({ username: username })

  const lastMailAt = user.mailSent
  if (!lastMailAt || new Date() - lastMailAt > FiveHrs) {
    const info = await transporter.sendMail({
      from: '"Kaustubh Murumkar " <kaustubh.murumkar@gmail.com>',
      to: user.email,
      subject: ' There are some Faulty nodes in your Dashboard ',
      html:
      `Dear ${user.username}, please check your dashboard there may be some faulty nodes present`
    })
    User.findOneAndUpdate({ username: user.user }, { mailSent: Date.now() })
  }
}

module.exports = reportMail
