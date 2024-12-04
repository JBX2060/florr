import { MongoClient } from "mongodb";
import { readFileSync } from "fs";
import { join, dirname } from "path";
import { fileURLToPath } from "url";

const uri = process.env.MONGODB_URI;
const client = new MongoClient(uri);

async function main() {
    try {
        await client.connect();
        const db = client.db("rrolf");
        const users_collection = db.collection("users");

        const __filename = fileURLToPath(import.meta.url);
        const __dirname = dirname(__filename);
        const usersPath = join(__dirname, "..", "users.json");
        const usersData = JSON.parse(readFileSync(usersPath, "utf-8"));

        const bulk_ops = [];

        for (const userData of usersData) {
            const petalUpdates = {};
            if (userData.petals) {
                for (let [petalId, count] of Object.entries(userData.petals)) {
                    if (petalId.startsWith("5:")) {
                        if (Number(petalId.split(':')[1]) > 1) {
                            const [id, rarity] = petalId.split(':');
                            const newPetalId = `${id}:${Number(rarity) - 1}`;
                            petalUpdates[`petals.${newPetalId}`] = count;
                        } else {
                            petalUpdates[`petals.${petalId}`] = count;
                        }
                    }
                }
            }

            if (Object.keys(petalUpdates).length > 0) {
                bulk_ops.push({
                    updateOne: {
                        filter: { _id: userData._id },
                        update: { $set: petalUpdates }
                    }
                });
            }
        }

        if (bulk_ops.length > 0) {
            const result = await users_collection.bulkWrite(bulk_ops);
            console.log(`Completed! Updated ${result.modifiedCount} users`);
        } else {
            console.log("No updates required");
        }
    } finally {
        await client.close();
    }
}

main().catch(console.error);
