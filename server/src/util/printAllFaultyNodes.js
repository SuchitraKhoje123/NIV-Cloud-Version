const Node = require('../models/nodes')

async function printAllFaultyNodes () {
  const nodes = await Node.find()
  const allNodes = nodes.map(node => node.uid)

}

module.exports = printAllFaultyNodes
