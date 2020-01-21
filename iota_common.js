const iotaCommonApi = require('./build/Release/iota_common.node')

/**
 * Do Proof of Work on trytes
 * @param {string} trytes - Input trytes value
 * @param {number} mwm - (optional) Min Weight Magnitude
 * @returns {string} Proof of Work
 **/
const powTrytesFunc = (trytes, mwm) => {
	return new Promise((resolve, reject) => {
		try {
			const pow = iotaCommonApi.powTrytes(trytes, mwm || 14)
			resolve(pow)
		} catch (err) {
			reject(err)
		}
	})
}

/**
 * Do Proof of Work on a bundle
 * @param {Array<string>} trytes - Input transaction trytes
 * @param {string} trunk - Trunk hash
 * @param {string} branch - Bundle hash
 * @param {number} mwm - (optional) Min Weight Magnitude
 * @returns {Array<string>} Output transaction trytes
 **/
const powBundleFunc = (trytes, trunk, branch, mwm) => {
	return new Promise((resolve, reject) => {
		try {
			const transactions = iotaCommonApi.powBundle(trytes, trunk, branch, mwm || 14)
			resolve(transactions)
		} catch (err) {
			reject(err)
		}
	})
}

/**
 * Generate address in trytes
 * @param {string} seed - Seed in trytes
 * @param {number} index - Address index
 * @param {number} security - (optional) Target security
 * @returns {string} Address in trytes
 **/
const genAddressTrytesFunc = (seed, index, security) => {
	return new Promise((resolve, reject) => {
		try {
			const address = iotaCommonApi.genAddressTrytes(seed, index, security || 2)
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
 * @param {number} security - (optional) Target security
 * @returns {Int8Array} Address in trits
 **/
const genAddressTritsFunc = (seed, index, security) => {
	return new Promise((resolve, reject) => {
		try {
			const address = iotaCommonApi.genAddressTrits(seed, index, security || 2)
			resolve(address)
		} catch (err) {
			reject(err)
		}
	})
}

/**
 * Generate signature in trytes
 * @param {string} seed - Seed in trytes
 * @param {number} index - Signature index
 * @param {number} security - (optional) Target security
 * @param {string} bundle - Bundle hash in trytes
 * @returns {string} Signature in trytes
 **/
const genSignatureTrytesFunc = (seed, index, security, bundle) => {
	return new Promise((resolve, reject) => {
		try {
			const signature = iotaCommonApi.genSignatureTrytes(seed, index, security || 2, bundle)
			resolve(signature)
		} catch (err) {
			reject(err)
		}
	})
}

/**
 * Generate signature in trits
 * @param {Int8Array} seed - Seed in trits
 * @param {number} index - Signature index
 * @param {number} security - (optional) Target security
 * @param {Int8Array} bundle - Bundle hash in trits
 * @returns {Int8Array} Signature in trits
 **/
const genSignatureTritsFunc = (seed, index, security, bundle) => {
	return new Promise((resolve, reject) => {
		try {
			const signature = iotaCommonApi.genSignatureTrits(seed, index, security || 2, bundle)
			resolve(signature)
		} catch (err) {
			reject(err)
		}
	})
}

/**
 * Transaction hash
 * @param {string} trytes - Transaction trytes
 * @returns {string} Hash trytes
 **/
const transactionHashFunc = (trytes) => {
	return new Promise((resolve, reject) => {
		try {
			const hash = iotaCommonApi.transactionHash(trytes)
			resolve(hash)
		} catch (err) {
			reject(err)
		}
	})
}

/**
 * Mines a bundle hash that minimizes the risks of a brute force signature forging attack
 * @param {Int8Array} bundleNormalizedMax - Bundle hash created by taking the maximum of each bytes of each already signed bundle hashes
 * @param {number} security - (optional) Target security
 * @param {Int8Array} essence - Bundle essence
 * @param {number} essenceLength - Bundle essence length
 * @param {number} count - Iteration count
 * @param {number} nprocs - Number of processors to run the miner on - 0 to use them all
 * @param {number} miningThreshold
 * @param {number} fullySecure - Flag indicating if 13s are allowed in non-signed normalized fragments. This enables Ledger support.
 * @returns {number} Best fitting index
 **/
const bundleMiner = (bundleNormalizedMax, security, essence, essenceLength, count, nprocs, miningThreshold, fullySecure) => {
	return new Promise((resolve, reject) => {
		try {
			const index = iotaCommonApi.bundleMiner(bundleNormalizedMax, security || 2, essence, essenceLength, count, nprocs || 0, miningThreshold, fullySecure)
			resolve(index)
		} catch (err) {
			reject(err)
		}
	})
}

module.exports = {
	powTrytesFunc,
	powBundleFunc,
	genAddressTrytesFunc,
	genAddressTritsFunc,
	genSignatureTrytesFunc,
	genSignatureTritsFunc,
	transactionHashFunc,
	bundleMiner
}
