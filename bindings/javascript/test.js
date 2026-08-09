import { stringify, validate } from './index.js';

async function runTests() {
    try {
        console.log("Running JS tests...");
        const out = await stringify({ server: { port: 8080 } });
        if (!out.includes("mocked_pxcf_string")) {
            throw new Error("stringify failed");
        }
        
        const valid = await validate("server: 8080");
        console.log("Validate result:", valid);
        
        console.log("All JS tests passed!");
    } catch (e) {
        console.error("Test failed:", e);
        process.exit(1);
    }
}

runTests();
