const setupTelex = require('telek')

async function initServer () {
  setupTelex('projin', {
    dbUrl: process.env.DATABASE_URL,
    aTok: process.env.ACCESS_TOKEN
  }, true, 'log')
}

module.exports = initServer
