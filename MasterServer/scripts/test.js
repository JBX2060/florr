import { MongoClient } from "mongodb";

const uri = process.env.MONGODB_URI;

const client = new MongoClient(uri);

const test = async () => {
    await client.connect();
    const db = client.db("rrolf");
    const users_collection = db.collection("users");

    const query = {
        username: "523fa9e8-2589-46c6-ad29-1e13fb7259ab",
    };
    const user = await users_collection.findOne(query);
    console.log(user);

    await client.close();
};

test();