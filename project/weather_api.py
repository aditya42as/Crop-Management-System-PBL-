# weather API module


import requests

WEATHERAPI_KEY = "edce00395091489b9f7112452251611"

def fetch_weather_forecast(city="Delhi", days=3):
    """
    Fetch forecast from WeatherAPI.com (free tier supports up to 3 days)
    """
    url = f"https://api.weatherapi.com/v1/forecast.json?key={WEATHERAPI_KEY}&q={city}&days={days}&aqi=no&alerts=no"

    try:
        r = requests.get(url, timeout=5)
        data = r.json()

        if "error" in data:
            return {"ok": False, "error": data["error"]["message"]}

        forecasts = []
        for d in data["forecast"]["forecastday"]:
            forecasts.append({
                "date": d["date"],
                "max_temp": d["day"]["maxtemp_c"],
                "min_temp": d["day"]["mintemp_c"],
                "avg_temp": d["day"]["avgtemp_c"],
                "humidity": d["day"]["avghumidity"],
                "rain_chance": d["day"]["daily_chance_of_rain"],
                "condition": d["day"]["condition"]["text"]
            })

        return {
            "ok": True,
            "city": data["location"]["name"],
            "forecast": forecasts
        }

    except Exception as e:
        return {"ok": False, "error": str(e)}
