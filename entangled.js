const entangledApi = require('./build/Release/entangled.node')

/**
 * Do Proof of Work on trytes
 * @param {string} trytes - Input trytes value
 * @param {number} mwm - (optional) Min Weight Magnitude
 * @returns {string} Proof of Work
 **/
const powTrytesFunc = (trytes, mwm) => {
	return new Promise((resolve, reject) => {
		try {
			const pow = entangledApi.powTrytes(trytes, mwm || 14)
			resolve(pow)
		} catch (err) {
			reject(err)
		}
	})
}

/**
 * Generate address in trytes
 * @param {string} seed - Seed in trytes
 * @param {number} index - Address index
 * @param {number} mwm - (optional) Target security
 * @returns {string} Address in trytes
 **/
const genAddressTrytesFunc = (seed, index, security) => {
	return new Promise((resolve, reject) => {
		try {
			const address = entangledApi.genAddressTrytes(seed, index, security || 2)
			resolve(address)
		} catch (err) {
			reject(err)
		}
	})
}

/**
 * Generate address in trits
 * @param {Int8Array} seed - Seed in trits
 * @param {number} index - Address index
 * @param {number} mwm - (optional) Target security
 * @returns {Int8Array} Address in trits
 **/
const genAddressTritsFunc = (seed, index, security) => {
	return new Promise((resolve, reject) => {
		try {
			const address = entangledApi.genAddressTrits(seed, index, security || 2)
			resolve(address)
		} catch (err) {
			reject(err)
		}
	})
}

module.exports = {
	powTrytesFunc,
	genAddressTrytesFunc,
	genAddressTritsFunc,
}
