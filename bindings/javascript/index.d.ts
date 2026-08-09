export interface PxcfConfig {
    [key: string]: any;
}

/**
 * Initialize the PXCF WebAssembly parser asynchronously.
 */
export function initPxcf(): Promise<any>;

/**
 * Parses a PXCF formatted string into a JavaScript Object.
 * @param sourceString The PXCF string.
 */
export function parse(sourceString: string): Promise<PxcfConfig>;
