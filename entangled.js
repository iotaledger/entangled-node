const entangledApi = require('./build/Release/entangled.node')

/**
 * Do Proof of Work
 * @param {string} trytes - Input trytes value
 * @param {number} mwm - (optional) Min Weight Magnitude
 * @returns {string} Proof of Work
 **/
const powFunc = (trytes, mwm) => {
	return new Promise((resolve, reject) => {
		try {
			const pow = entangledApi.pow(trytes, mwm || 14)
			resolve(pow)
		} catch (err) {
			reject(err)
		}
	})
}

/**
 * Generate address
 * @param {string} seed - Seed
 * @param {number} index - Address index
 * @param {number} mwm - (optional) Target security
 * @returns {string} Address
 **/
const genFunc = (seed, index, security) => {
	return new Promise((resolve, reject) => {
		try {
			const address = entangledApi.gen(seed, index, security || 2)
			resolve(address)
		} catch (err) {
			reject(err)
		}
	})
}

module.exports = {
	powFunc,
	genFunc
}
