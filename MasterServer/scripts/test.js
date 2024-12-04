import { MongoClient } from "mongodb";

const uri = process.env.MONGODB_URI;

const client = new MongoClient(uri);

const test = async () => {
    await client.connect();
    const db = client.db("rrolf");
    const users_collection = db.collection("users");

    const query = {
        username: "1747d712-4f4e-471c-a887-106311f1e0f1",
    };
    const user = await users_collection.findOne(query);
    console.log(user);

    await client.close();
};

test();