import { MongoClient } from "mongodb";
import { readFileSync } from "fs";
import { join, dirname } from "path";
import { fileURLToPath } from "url";

const uri = process.env.MONGODB_URI;
const client = new MongoClient(uri);

async function reset_to_file() {
    try {
        await client.connect();
        const db = client.db("rrolf");
        const users_collection = db.collection("users");

        const __filename = fileURLToPath(import.meta.url);
        const __dirname = dirname(__filename);
        const usersPath = join(__dirname, "..", "users.json");
        
        const userData = JSON.parse(readFileSync(usersPath, "utf-8"));
        await users_collection.drop().catch(() => console.log("Collection doesn't exist, creating new one"));
        
        if (Array.isArray(userData)) {
            console.log("working...");
            await users_collection.insertMany(userData);
        } else {
            await users_collection.insertOne(userData);
        }
        
        console.log("Database reset successfully with users.json data");
    } catch (error) {
        console.error("Error resetting database:", error);
        throw error;
    } finally {
        await client.close();
    }
}

reset_to_file();