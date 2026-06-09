const express = require("express");
const mysql = require("mysql2");
const bodyParser = require("body-parser");
const path = require("path");
const { send } = require("process");
const DISCORD_WEBHOOK_URL = "https://discordapp.com/api/webhooks/1446195125607731341/SIfEfAwOqUlthSjchcRh7MlEy5gitrzY9L1KuZtfDCHZqY5Kb6s6E1D4buSeTxYn_AlY"

const app = express();
app.use(bodyParser.json());

app.use(express.static("public"));

const db = mysql.createConnection({
    host: "localhost",
    user: "root",
    password: "061011",
    database: "weather"
});

db.connect(err => {
    if (err) {
        console.error("MySQL error:", err);
    } else {
        console.log("MySQL connected!");
    }
});

app.post("/api/upload", (req, res) => {
    const data = req.body;

    const sql = `
        INSERT INTO readings
        (temperature, humidity, pressure, altitude, co2, voc, wind_speed)
        VALUES (?, ?, ?, ?, ?, ?, ?)
    `;

    const values = [
        data.temperature,
        data.humidity,
        data.pressure,
        data.altitude,
        data.co2,
        data.voc,
        data.wind_speed,
        console.log(data.humidity)
        
    ];

    db.query(sql, values, (err) => {
        if (err) {
            console.error("Insert error:", err);
            return res.status(500).send("DB insert error");
        }
        if (data.humidity > 50) {
            sendDiscordAlert(`Warning, Humidity is too high at Uunii's Station, it is at, ${data.humidity}%`)
            
        }
        res.send("OK");
    });
});

app.get("/api/readings", (req, res) => {
    db.query(
        "SELECT * FROM readings ORDER BY id DESC LIMIT 50",
        (err, results) => {
            if (err) {
                console.error("Fetch error:", err);
                return res.status(500).send("DB error");
            }
            res.json(results);
        }
    );
});

app.listen(3000, () => {
    console.log("Server running at http://localhost:3000");
});

async function sendDiscordAlert(message) {
    try {
        await fetch(DISCORD_WEBHOOK_URL, {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ content: message })
        });
        console.log("Discord alert sent!");
    } catch (err) {
        console.error("Discord alert failed:", err);
    }
}