require('dotenv').config()

const express = require('express')
const app = express()
const mongoose = require('mongoose')
const cors = require('cors')
const path = require('path')
const compression = require('compression')
const initServer = require('./lib/InitServer')
// const nodeCron = require('node-cron')
// const printAllFaultyNodes = require('./util/printAllFaultyNodes')

mongoose.connect(
  process.env.DATABASE_URL,
  {
    useNewUrlParser: true,
    useUnifiedTopology: true,
    useFindAndModify: false,
    useCreateIndex: true
  })

app.use(express.json())
app.use(cors())
app.use(compression())

app.use(express.static(path.resolve(__dirname, 'public')))

const db = mongoose.connection
db.on('error', (e) => console.log(e))
db.once('open', () => console.log('Connected to Database'))

const nodeRouter = require('./routes/nodes')
app.use('/node', nodeRouter)

const userRouter = require('./routes/users')
app.use('/user', userRouter)

const writeRouter = require('./routes/write')
app.use('/write', writeRouter)

app.get('/version', (_req, res) => {
  res.json({ version: '0.0.4' })
})

app.get(/.*/, (_req, res) => {
  res.sendFile(path.resolve(__dirname, 'public', 'index.html'))
})

app.set('port', process.env.PORT || 3000)

// Adding cron jobs
// const cronstring = (process.env.NODE_ENV === 'production') ? '* */5 * * *' : '*/10 * * * * *'
// const task = nodeCron.schedule(cronstring, () => {
//   // printAllFaultyNodes()
// })
// task.start()

app.listen(app.get('port'), initServer)

module.exports = app
