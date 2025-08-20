from flask import Flask, request, render_template
import numpy as np
import pandas as pd
import joblib
from tensorflow.keras.models import load_model

# Load model & scaler
model = load_model("ann_model.h5")
scaler = joblib.load("scaler.pkl")

# Min & Max moisture from training data
min_moisture = 10.0
max_moisture = 80.0

app = Flask(__name__)

@app.route('/')
def home():
    return render_template("index.html")

@app.route('/predict', methods=['POST'])
def predict():
    try:
        # Extract values from form
        air_humidity = float(request.form['air_humidity'])
        air_temp = float(request.form['air_temp'])
        soil_temp = float(request.form['soil_temp'])
        water_level = float(request.form['water_level'])
        uv_light = float(request.form['uv_light'])

        # DataFrame with correct column names
        df = pd.DataFrame([[
            air_humidity, air_temp, soil_temp, water_level, uv_light
        ]], columns=['air_humidity', 'air_temp', 'soil_temp', 'water_level', 'uv_light'])

        # Rename columns to match training-time names
        df.rename(columns={
            'air_temp': 'air_temperature',
            'soil_temp': 'soil_temperature'
        }, inplace=True)

        # Order columns to match training
        expected_order = ['air_humidity', 'air_temperature', 'soil_temperature', 'water_level', 'uv_light']
        df = df[expected_order]

        # Scale input
        scaled_input = scaler.transform(df)

        # Predict
        scaled_output = model.predict(scaled_input).flatten()[0]
        predicted_moisture = min_moisture + scaled_output * (max_moisture - min_moisture)

        return render_template("index.html",
                               prediction=round(predicted_moisture, 2),
                               air_humidity=air_humidity,
                               air_temp=air_temp,
                               soil_temp=soil_temp,
                               water_level=water_level,
                               uv_light=uv_light,
                               min_moisture=min_moisture,
                               max_moisture=max_moisture)

    except Exception as e:
        return f"Error: {e}"

if __name__ == "__main__":
    app.run(debug=True)
