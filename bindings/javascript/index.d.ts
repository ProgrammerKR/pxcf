/**
 * Parses a PXCF formatted string into a JavaScript Object.
 * @param {string} sourceString 
 * @returns {Promise<any>}
 */
export function parse(sourceString: string): Promise<any>;

/**
 * Serializes a JavaScript Object to a PXCF formatted string.
 * @param {any} obj 
 * @returns {Promise<string>}
 */
export function stringify(obj: any): Promise<string>;

/**
 * Validates a PXCF formatted string.
 * @param {string} sourceString 
 * @returns {Promise<boolean>}
 */
export function validate(sourceString: string): Promise<boolean>;
