// Type definitions for entangled-node

export function powTrytesFunc(trytes: string, mwm: number): Promise<String>
export function genAddressTrytesFunc(seed: string, index: number, security: number): Promise<String>
