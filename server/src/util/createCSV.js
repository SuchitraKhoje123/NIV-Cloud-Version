const fastcsv = require('fast-csv')

const createCSV = async (data, res) => {
  fastcsv
    .write(data, { headers: true })
    .on('finish', function () {
    })
    .pipe(res)
}

module.exports = createCSV
