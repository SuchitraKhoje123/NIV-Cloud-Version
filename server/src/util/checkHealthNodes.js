const Node = require('../models/nodes')

async function checkHealthNodes (reading) {
  const node = await Node.findOne({ uid: reading.uid })

  const pressureCondition = reading.pressure < node.pressureRange.min || reading.pressure > node.pressureRange.max
  const temperatureCondition = reading.temperature < node.temperatureRange.min || reading.temperature > node.temperatureRange.max
  const humidityCondition = reading.humidity < node.humidityRange.min || reading.humidity > node.humidityRange.max
  const co2Condition = reading.co2 < node.co2Range.min || reading.co2 > node.co2Range.max

  return !(pressureCondition || temperatureCondition || humidityCondition || co2Condition)
}

module.exports = checkHealthNodes
