from flask import Flask, render_template, request, redirect, url_for, session, jsonify, flash
import os
from statistics import mean, stdev
from weather_api import fetch_weather_forecast

app = Flask(__name__)
app.secret_key = "change_this_secret"
ROOT = os.path.dirname(os.path.abspath(__file__))

def auth_file(): return os.path.join(ROOT, "Authenticate.txt")
def crop_file(u): return os.path.join(ROOT, f"{u}_Crop_data.txt")
def weather_file(u): return os.path.join(ROOT, f"{u}_weather.txt")
def history_file(u): return os.path.join(ROOT, f"{u}_combined_history.txt")
def todo_file(u): return os.path.join(ROOT, f"{u}_todo.txt")

def load_users():
    users = {}
    try:
        with open(auth_file(),"r") as f:
            for line in f:
                line=line.strip()
                if line:
                    parts = line.split()
                    if len(parts) >= 2:
                        users[parts[0]] = parts[1]
    except FileNotFoundError:
        pass
    return users

def save_users(users):
    with open(auth_file(),"w") as f:
        for u,p in users.items():
            f.write(f"{u} {p}\n")

def read_crops(user):
    arr=[]
    try:
        with open(crop_file(user),"r") as f:
            for line in f:
                a=line.strip().split("|")
                if len(a)>=8:
                    arr.append({
                        "name":a[0], "type":a[1], "season":a[2], "harvest":a[3],
                        "waterReq":a[4], "fertilizer":a[5], "price":float(a[6]), "area":float(a[7])
                    })
    except FileNotFoundError:
        pass
    return arr

def write_crops(user,rows):
    with open(crop_file(user),"w") as f:
        for r in rows:
            f.write("|".join([
                r["name"],r["type"],r["season"],r["harvest"],
                r["waterReq"],r["fertilizer"],str(r["price"]),str(r["area"])
            ])+"\n")

def read_weather(user):
    arr=[]
    try:
        with open(weather_file(user),"r") as f:
            for line in f:
                a=line.strip().split("|")
                if len(a)>=4:
                    arr.append({
                        "date":a[0],
                        "temperature":float(a[1]),
                        "humidity":float(a[2]),
                        "rainfall":float(a[3])
                    })
    except FileNotFoundError:
        pass
    return arr

def write_weather(user,rows):
    with open(weather_file(user),"w") as f:
        for r in rows:
            f.write("|".join([r["date"],str(r["temperature"]),str(r["humidity"]),str(r["rainfall"])])+"\n")

def read_history(user):
    arr=[]
    try:
        with open(history_file(user),"r") as f:
            for line in f:
                a=line.strip().split("|")
                if len(a)>=12:
                    arr.append({
                        "cropName":a[0],"harvestDate":a[1],
                        "area":float(a[2]),"yieldPerArea":float(a[3]),
                        "totalYield":float(a[4]),"marketPrice":float(a[5]),
                        "revenue":float(a[6]),"cost":float(a[7]),
                        "profit":float(a[8]),"avgTemp":float(a[9]),
                        "avgHumidity":float(a[10]),"totalRainfall":float(a[11])
                    })
    except FileNotFoundError:
        pass
    return arr

def write_history(user, rows):
    with open(history_file(user),"w") as f:
        for r in rows:
            f.write("|".join([
                r["cropName"], r["harvestDate"], str(r["area"]), str(r["yieldPerArea"]),
                str(r["totalYield"]), str(r["marketPrice"]), str(r["revenue"]), str(r["cost"]),
                str(r["profit"]), str(r["avgTemp"]), str(r["avgHumidity"]), str(r["totalRainfall"])
            ]) + "\n")

def read_todo(user):
    arr=[]
    try:
        with open(todo_file(user),"r") as f:
            for line in f:
                a=line.strip().split("|",1)
                if len(a)==2:
                    arr.append({"priority":int(a[0]),"desc":a[1]})
    except FileNotFoundError:
        pass
    return arr

def write_todo(user, tasks):
    with open(todo_file(user),"w") as f:
        for t in tasks:
            f.write(f"{t['priority']}|{t['desc']}\n")

def _linear_fit(values, days):
    n=len(values)
    if n == 0:
        return [0.0]*days, 0.0
    x_mean = (n-1)/2
    y_mean = mean(values)
    num=0; den=0
    for i,y in enumerate(values):
        num += (i-x_mean)*(y-y_mean)
        den += (i-x_mean)*(i-x_mean)
    b = num/den if den!=0 else 0
    a = y_mean - b*x_mean
    preds=[]
    for s in range(1,days+1):
        preds.append(a + b*(n-1+s))
    residuals=[(y-(a+b*i)) for i,y in enumerate(values)]
    sigma = stdev(residuals) if len(residuals)>=2 else 0
    return preds, sigma

def offline_predict(user, days=1):
    w = read_weather(user)
    temps=[x["temperature"] for x in w]
    hums =[x["humidity"] for x in w]
    rains=[x["rainfall"] for x in w]
    def fallback(v):
        if len(v)==0: return [0]*days,0
        return [mean(v)]*days, (stdev(v) if len(v)>=2 else 0)
    if len(temps)>=3: t,s1=_linear_fit(temps,days)
    else:             t,s1=fallback(temps)
    if len(hums)>=3:  h,s2=_linear_fit(hums,days)
    else:             h,s2=fallback(hums)
    if len(rains)>=3: r,s3=_linear_fit(rains,days)
    else:             r,s3=fallback(rains)
    out=[]
    for i in range(days):
        out.append({
            "day_offset": i+1,
            "temperature": round(t[i],2),
            "humidity": round(h[i],2),
            "rainfall": round(r[i],2),
            "conf_std": round((s1+s2+s3)/3, 3)
        })
    return out

def apply_search_sort(rows, q, sort_key, order, columns):
    if q:
        qlow = q.lower()
        rows = [r for r in rows if any(qlow in str(r.get(col,"")).lower() for col in columns)]
    if sort_key:
        reverse = (order == "desc")
        try:
            rows = sorted(rows, key=lambda x: (x.get(sort_key) is None, x.get(sort_key)), reverse=reverse)
        except Exception:
            rows = sorted(rows, key=lambda x: str(x.get(sort_key,"")), reverse=reverse)
    return rows

@app.route("/")
def index():
    return redirect("/login" if "username" not in session else "/dashboard")

@app.route("/login",methods=["GET","POST"])
def login():
    if request.method=="POST":
        users = load_users()
        u = request.form.get("username","").strip()
        p = request.form.get("password","").strip()
        if u in users and users[u] == p:
            session["username"] = u
            return redirect("/dashboard")
        flash("Invalid credentials","danger")
    return render_template("login.html")

@app.route("/signup",methods=["POST"])
def signup():
    u = request.form.get("username","").strip()
    p = request.form.get("password","").strip()
    if not u or not p:
        flash("Provide username and password","warning")
        return redirect("/login")
    users = load_users()
    if u in users:
        flash("Username exists","warning")
        return redirect("/login")
    users[u] = p
    save_users(users)
    open(crop_file(u),"a").close()
    open(weather_file(u),"a").close()
    open(history_file(u),"a").close()
    open(todo_file(u),"a").close()
    flash("Account created. Log in.","success")
    return redirect("/login")

@app.route("/logout")
def logout():
    session.pop("username", None)
    return redirect("/login")

@app.route("/dashboard")
def dashboard():
    if "username" not in session:
        return redirect("/login")
    u = session["username"]
    crops = len(read_crops(u))
    we = len(read_weather(u))
    hi = len(read_history(u))
    to = len(read_todo(u))
    return render_template("dashboard.html", user=u, crops=crops, weather=we, history=hi, tasks=to)

@app.route("/crops")
def crops_page():
    if "username" not in session:
        return redirect("/login")
    user = session["username"]
    crops = read_crops(user)
    return render_template("crops.html", user=user, crops=crops)

@app.route("/crops/add", methods=["POST"])
def crops_add():
    if "username" not in session:
        return jsonify({"error":"unauth"}),401
    user = session["username"]
    data = request.json
    if not data:
        return jsonify({"error":"bad request"}),400
    crops = read_crops(user)
    for c in crops:
        if c["name"] == data.get("name",""):
            return jsonify({"error":"exists"}),409
    crops.append({
        "name": data.get("name",""),
        "type": data.get("type",""),
        "season": data.get("season",""),
        "harvest": data.get("harvest",""),
        "waterReq": data.get("waterReq",""),
        "fertilizer": data.get("fertilizer",""),
        "price": float(data.get("price",0)),
        "area": float(data.get("area",0))
    })
    write_crops(user, crops)
    return jsonify({"ok":True})

@app.route("/crops/delete", methods=["POST"])
def crops_delete():
    if "username" not in session:
        return jsonify({"error":"unauth"}),401
    user = session["username"]
    name = request.json.get("name","")
    crops = read_crops(user)
    new = [c for c in crops if c["name"] != name]
    write_crops(user, new)
    return jsonify({"ok":True})

@app.route("/crops/edit", methods=["POST"])
def crops_edit():
    if "username" not in session:
        return jsonify({"error":"unauth"}),401
    user = session["username"]
    data = request.json
    name = data.get("name","")
    crops = read_crops(user)
    for c in crops:
        if c["name"] == name:
            c["type"] = data.get("type", c["type"])
            c["season"] = data.get("season", c["season"])
            c["harvest"] = data.get("harvest", c["harvest"])
            c["waterReq"] = data.get("waterReq", c["waterReq"])
            c["fertilizer"] = data.get("fertilizer", c["fertilizer"])
            c["price"] = float(data.get("price", c["price"]))
            c["area"] = float(data.get("area", c["area"]))
            write_crops(user, crops)
            return jsonify({"ok":True})
    return jsonify({"error":"not found"}),404

@app.route("/crops/view")
def crops_view():
    if "username" not in session:
        return redirect("/login")
    user = session["username"]
    rows = read_crops(user)
    q = request.args.get("q", "").strip()
    sort = request.args.get("sort", "name")
    order = request.args.get("order", "asc")
    columns = ["name","type","season","harvest","waterReq","fertilizer","price","area"]
    rows = apply_search_sort(rows, q, sort, order, columns)
    return render_template("view_table.html", title="Crops", columns=columns, rows=rows, q=q, sort=sort, order=order)

@app.route("/weather")
def weather_page():
    if "username" not in session:
        return redirect("/login")
    user = session["username"]
    recs = read_weather(user)
    return render_template("weather.html", user=user, recs=recs)

@app.route("/weather/add", methods=["POST"])
def weather_add():
    if "username" not in session:
        return jsonify({"error":"unauth"}),401
    user = session["username"]
    data = request.json
    recs = read_weather(user)
    recs.append({"date": data.get("date",""), "temperature": float(data.get("temperature",0)),
                 "humidity": float(data.get("humidity",0)), "rainfall": float(data.get("rainfall",0))})
    write_weather(user, recs)
    return jsonify({"ok":True})

@app.route("/weather/delete", methods=["POST"])
def weather_delete():
    if "username" not in session:
        return jsonify({"error":"unauth"}),401
    user = session["username"]
    date = request.json.get("date","")
    recs = read_weather(user)
    recs = [r for r in recs if r["date"] != date]
    write_weather(user, recs)
    return jsonify({"ok":True})

@app.route("/weather/predict")
def predict_offline():
    if "username" not in session:
        return jsonify({"error":"unauth"}),401
    user = session["username"]
    try:
        days = int(request.args.get("days","1"))
    except Exception:
        days = 1
    days = max(1,min(7,days))
    out = offline_predict(user, days)
    return jsonify({"ok":True,"predictions":out})

@app.route("/weather/api_predict")
def api_predict():
    if "username" not in session:
        return jsonify({"error":"unauth"}),401
    city = request.args.get("city","Delhi")
    try:
        days = int(request.args.get("days",3))
    except Exception:
        days = 3
    days = max(1,min(3,days))
    result = fetch_weather_forecast(city, days)
    return jsonify(result)

@app.route("/weather/view")
def weather_view():
    if "username" not in session:
        return redirect("/login")
    user = session["username"]
    rows = read_weather(user)
    q = request.args.get("q", "").strip()
    sort = request.args.get("sort", "date")
    order = request.args.get("order", "asc")
    columns = ["date","temperature","humidity","rainfall"]
    rows = apply_search_sort(rows, q, sort, order, columns)
    return render_template("view_table.html", title="Weather", columns=columns, rows=rows, q=q, sort=sort, order=order)

@app.route("/history")
def history_page():
    if "username" not in session:
        return redirect("/login")
    user = session["username"]
    rows = read_history(user)
    return render_template("history.html", user=user, rows=rows)

@app.route("/history/add_basic", methods=["POST"])
def history_add_basic():
    if "username" not in session:
        return jsonify({"error":"unauth"}),401
    user = session["username"]
    d = request.json
    rows = read_history(user)
    area = float(d.get("area",0)); ypa = float(d.get("yieldPerArea",0)); price = float(d.get("marketPrice",0)); cost = float(d.get("cost",0))
    total = area * ypa; revenue = total * price; profit = revenue - cost
    rows.append({"cropName": d.get("cropName",""), "harvestDate": d.get("harvestDate",""),
                 "area": area, "yieldPerArea": ypa, "totalYield": total, "marketPrice": price,
                 "revenue": revenue, "cost": cost, "profit": profit, "avgTemp":0, "avgHumidity":0, "totalRainfall":0})
    write_history(user, rows)
    return jsonify({"ok":True})

@app.route("/history/estimate_and_add", methods=["POST"])
def history_estimate_and_add():
    if "username" not in session:
        return jsonify({"error":"unauth"}),401
    user = session["username"]
    d = request.json
    area = float(d.get("area",0)); base = float(d.get("baseYieldPerArea",0)); price = float(d.get("marketPrice",0)); cost = float(d.get("cost",0))
    est = offline_predict(user, 1)[0] if False else None
    est = None
    from math import isfinite
    recs = read_weather(user)
    temps = [r["temperature"] for r in recs]
    hums = [r["humidity"] for r in recs]
    rains = [r["rainfall"] for r in recs]
    if len(temps) >= 3:
        tpred, _ = _linear_fit(temps, 1)
        adjusted = base * 1.0
        adjusted = tpred[0] * (base / (base if base!=0 else 1))
    else:
        adjusted = base
    # fallback simple calculation using offline_predict for consistency
    est_full = offline_predict(user, 1)[0]
    rows = read_history(user)
    rows.append({"cropName": d.get("cropName",""), "harvestDate": d.get("harvestDate",""),
                 "area": area, "yieldPerArea": est_full["adjusted_yield_per_area"] if "adjusted_yield_per_area" in est_full else est_full.get("yieldPerArea", area),
                 "totalYield": est_full.get("totalYield", area*base), "marketPrice": price,
                 "revenue": est_full.get("revenue", area*base*price), "cost": cost, "profit": est_full.get("profit", 0),
                 "avgTemp": est_full.get("avgTemp",0), "avgHumidity": est_full.get("avgHumidity",0), "totalRainfall": est_full.get("totalRainfall",0)})
    write_history(user, rows)
    return jsonify({"ok":True, "estimate": est_full})

@app.route("/history/view")
def history_view():
    if "username" not in session:
        return redirect("/login")
    user = session["username"]
    rows = read_history(user)
    q = request.args.get("q", "").strip()
    sort = request.args.get("sort", "harvestDate")
    order = request.args.get("order", "desc")
    columns = ["cropName","harvestDate","area","yieldPerArea","totalYield","marketPrice","revenue","profit"]
    rows = apply_search_sort(rows, q, sort, order, columns)
    return render_template("view_table.html", title="History", columns=columns, rows=rows, q=q, sort=sort, order=order)

@app.route("/todo")
def todo_page():
    if "username" not in session:
        return redirect("/login")
    user = session["username"]
    tasks = read_todo(user)
    return render_template("todo.html", user=user, tasks=tasks)

@app.route("/todo/add", methods=["POST"])
def todo_add():
    if "username" not in session:
        return jsonify({"error":"unauth"}),401
    user = session["username"]
    d = request.json
    tasks = read_todo(user)
    tasks.append({"priority": int(d.get("priority",0)), "desc": d.get("desc","")})
    write_todo(user, tasks)
    return jsonify({"ok":True})

@app.route("/todo/complete", methods=["POST"])
def todo_complete():
    if "username" not in session:
        return jsonify({"error":"unauth"}),401
    user = session["username"]
    tasks = read_todo(user)
    if not tasks:
        return jsonify({"error":"empty"}),400
    top = max(tasks, key=lambda x: x["priority"])
    tasks.remove(top)
    write_todo(user, tasks)
    return jsonify({"ok":True, "completed": top})

@app.route("/todo/view")
def todo_view():
    if "username" not in session:
        return redirect("/login")
    user = session["username"]
    rows = read_todo(user)
    q = request.args.get("q", "").strip()
    sort = request.args.get("sort", "priority")
    order = request.args.get("order", "desc")
    columns = ["priority","desc"]
    rows = apply_search_sort(rows, q, sort, order, columns)
    return render_template("view_table.html",
                           title="To-Do",
                           columns=columns,
                           rows=rows,
                           q=q,
                           sort=sort,
                           order=order)


@app.route("/api/me")
def api_me():
    if "username" in session:
        return jsonify({"user": session["username"]})
    return jsonify({"user": None})

if __name__ == "__main__":
    app.run(debug=True)
