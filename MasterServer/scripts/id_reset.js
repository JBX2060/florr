import { MongoClient } from "mongodb";
const uri = process.env.MONGODB_URI;

const client = new MongoClient(uri);

const old_to_new_petal_mapping = {
    0: 0,   // none -> none
    1: 1,   // basic -> basic
    2: 2,   // pellet -> pellet
    3: 3,   // fossil -> fossil
    4: 4,   // stinger -> stinger
    5: -1,  // light -> removed
    6: 6,   // shell -> shell
    7: 7,   // peas -> peas
    8: 8,   // leaf -> leaf
    9: 9,   // egg -> egg
    10: 10, // magnet -> magnet
    11: 11, // uranium -> uranium
    12: 12, // feather -> feather
    13: 13, // azalea -> azalea
    14: 14, // bone -> bone
    15: 15, // web -> web
    16: 16, // seed -> seed
    17: 17, // gravel -> gravel
    18: 18, // club -> club
    19: 19, // crest -> crest
    20: 20, // droplet -> droplet
    21: 21, // beak -> beak
    22: 22, // lightning -> lightning
    23: 23, // third_eye -> third_eye
    24: 27, // mandible -> mandible (new position)
    25: 28, // wax -> wax (new position)
    26: 29, // sand -> sand (new position)
    27: 30, // mint -> mint (new position)
    28: -1, // cactus -> removed
    29: -1, // gold -> removed
    30: 31, // rice -> rice (new position)
    31: -1, // horn -> removed
    32: 32, // stick -> stick
    33: 33, // missile -> missile
    34: -1, // iris -> removed
    35: -1, // rock -> removed
    36: -1, // honey -> removed
    37: -1, // ocean_shell -> removed
    38: -1, // kelp -> removed
    39: -1, // fish_egg -> removed
    40: -1, // scales -> removed
    41: -1  // water -> removed
};

async function migrate_petal_ids() {
    try {
        await client.connect();
        console.log('Connected to MongoDB!');

        const db = client.db("rrolf");
        const collection = db.collection('users');

        const cursor = collection.find({
            "petals": { $exists: true }
        });

        let update_count = 0;
        let processed_count = 0;

        await cursor.forEach(async (doc) => {
            let modified = false;
            const new_petals = {};

            // (format: "id:rarity": count)
            for (const [key, count] of Object.entries(doc.petals)) {
                const [old_id, rarity] = key.split(':').map(Number);
                
                if (old_to_new_petal_mapping[old_id] !== undefined) {
                    const new_id = old_to_new_petal_mapping[old_id];
                    if (new_id === -1) {
                        const new_key = `1:${rarity}`;
                        new_petals[new_key] = (new_petals[new_key] || 0) + count;
                        modified = true;
                    } else {
                        const new_key = `${new_id}:${rarity}`;
                        new_petals[new_key] = count;
                        if (new_key !== key) {
                            modified = true;
                        }
                    }
                } else {
                    new_petals[key] = count;
                }
            }

            if (modified) {
                doc.petals = new_petals;
                await collection.replaceOne({ _id: doc._id }, doc);
                update_count++;
            }
            processed_count++;

            if (processed_count % 1000 === 0) {
                console.log(`Processed ${processed_count} documents...`);
            }
        });

        console.log(`Processed ${processed_count} documents`);
        console.log(`Updated ${update_count} documents`);

    } catch (err) {
        console.error('Error during migration:', err);
    } finally {
        await client.close();
        console.log('Migration completed');
    }
}

migrate_petal_ids().catch(console.error);
