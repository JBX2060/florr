import { MongoClient } from "mongodb";
const uri = process.env.MONGODB_URI;

const client = new MongoClient(uri);

const shift_rarities = async () => {
    let session;
    try {
        await client.connect();
        console.log('Connected to MongoDB!');

        session = client.startSession();
        await session.withTransaction(async () => {
            const db = client.db("rrolf");
            const collection = db.collection('users');

            const cursor = collection.find({
                "failed_crafts": { $exists: true }
            });

            let update_count = 0;
            let petal_count = 0;
            let processed_count = 0;

            for await (const doc of cursor) {
                let modified = false;
                const new_petals = {};

                for (const [key, count] of Object.entries(doc.petals)) {
                    const [id, rarity] = key.split(':').map(Number);
                    
                    if (rarity > 1) {
                        const new_key = `${id}:${rarity - 1}`;
                        new_petals[new_key] = count;
                        modified = true;
                        petal_count += count;
                    } else {
                        new_petals[key] = count;
                    }
                }

                if (modified) {
                    doc.failed_crafts = new_petals;
                    await collection.replaceOne({ _id: doc._id }, doc, { session });
                    update_count++;
                }
                processed_count++;

                if (processed_count % 1000 === 0) {
                    console.log(`Processed ${processed_count} documents...`);
                }
            }

            console.log('\nMigration Summary:');
            console.log(`Total documents processed: ${processed_count}`);
            console.log(`Documents updated: ${update_count}`);
            console.log(`Petals shifted down: ${petal_count}`);
        });
    } catch (err) {
        console.error('Error during rarity shift:', err);
    } finally {
        await session?.endSession();
        await client.close();
        console.log('\nRarity shift completed');
    }
};

shift_rarities().catch(console.error);