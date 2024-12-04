import { MongoClient } from "mongodb";

const uri = process.env.MONGODB_URI;
const client = new MongoClient(uri);

async function remove_exotic_petal() {
    let session;
    try {
        await client.connect();
        console.log('Connected to MongoDB!');

        session = client.startSession();
        await session.withTransaction(async () => {
            const db = client.db("rrolf");
            const collection = db.collection('users');

            const cursor = collection.find({ "petals": { $exists: true } });

            let update_count = 0;
            let processed_count = 0;

            for await (const doc of cursor) {
                let modified = false;
                const new_petals = {};

                for (const [key, count] of Object.entries(doc.petals)) {
                    const [old_id, rarity] = key.split(':').map(Number);
                    if (old_id === 1 && rarity >= 6) {
                        modified = true;
                        console.log("Found one!!");
                        continue;  
                    }
                    new_petals[key] = count;  
                }

                if (modified) {
                    await collection.updateOne(
                        { _id: doc._id },
                        { $set: { petals: new_petals } }
                    );
                    update_count++;
                }
                processed_count++;
            }
        });
    } catch (e) {
        console.error(e);
    } finally {
        await client.close();
    }
}
remove_exotic_petal();