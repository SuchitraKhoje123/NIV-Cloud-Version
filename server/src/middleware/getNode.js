const Node = require('../models/nodes')

async function getNode (req, res, next) {
  let node
  try {
    node = await Node.find({ uid: req.body.uid })
    if (node == null) {
      return res.status(404).json({ message: 'Cannot Find Node' })
    }
  } catch (err) {
    return res.status(404).json({ message: err.message })
  }
  req.node = node[0]
  next()
}

module.exports = getNode
