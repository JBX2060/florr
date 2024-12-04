import fs from 'fs/promises';
import { MongoClient } from "mongodb";
const uri = process.env.MONGODB_URI;

const client = new MongoClient(uri);

async function save_users_collection() {
  try {
    await client.connect();
    console.log('Connected to MongoDB!');

    const db = client.db("rrolf");
    const collection = db.collection("users");
    
    const users = await collection.find({}).toArray();
    
    await fs.writeFile('users.json', JSON.stringify(users, null, 2));
    
    console.log("Users collection saved to users.json");
  } catch (error) {
    console.error("Error saving users collection:", error);
  } finally {
    await client.close();
  }
}

save_users_collection().catch(console.error);