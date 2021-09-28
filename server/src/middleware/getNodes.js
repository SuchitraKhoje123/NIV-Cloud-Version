const Node = require('../models/nodes')

async function getNodes (req, _res, next) {
  let node
  try {
    node = await Node.find({
      user: req.body.user
    }).sort({
      datetime: -1
    })
    req.nodes = node
  } catch (e) {
    req.err = e.message
  }
  next()
}

module.exports = getNodes
