const jwt = require('jsonwebtoken')
const nodemailer = require('nodemailer')

const sendResetLink = async (user) => {
  try {
    const token = jwt.sign({
      username: user.username,
      id: user._id
    }, process.env.EMAIL_TOK)
    const transporter = nodemailer.createTransport({
      service: 'gmail',
      auth: {
        user: process.env.FROM_EMAIL,
        pass: process.env.FROM_PWD
      }
    })

    const info = await transporter.sendMail({
      from: '"Kaustubh Murumkar " <kaustubh.murumkar@gmail.com>',
      to: user.email,
      subject: ' Password Reset ',
      html: `<b> Hello ${user.username}</b>, you had requested for password reset <br>
      Here is your password reset link <br>
      ${process.env.HOST}:8080/passwordreset?token=${token}
      `
    })
    return info
  } catch (e) {
    return null
  }
}

const verifyToken = async (token) => {
  let usr
  jwt.verify(token, process.env.EMAIL_TOK, (err, user) => {
    if (err) throw new Error('Authentication error')
    usr = user
  })
  return usr
}

module.exports = {
  sendResetLink,
  verifyToken
}
