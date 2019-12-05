// Type definitions for entangled-node

export function powTrytesFunc(trytes: string, mwm: number): Promise<string>
export function powBundleFunc(trytes: Array<string>, trunk: string, branch: string, mwm: number): Promise<Array<string>>
export function genAddressTrytesFunc(seed: string, index: number, security: number): Promise<string>
export function genAddressTritsFunc(seed: Int8Array, index: number, security: number): Promise<Int8Array>
export function genSignatureTrytesFunc(seed: string, index: number, security: number, bundle: string): Promise<string>
export function genSignatureTritsFunc(seed: Int8Array, index: number, security: number, bundle: Int8Array): Promise<Int8Array>
export function transactionHashFunc(trytes: string): Promise<string>
export function bundleMiner(bundleNormalizedMax: Int8Array, security: number, essence: Int8Array, essenceLength: number, count: number, nprocs: number, miningThreshold: number): Promise<number>
