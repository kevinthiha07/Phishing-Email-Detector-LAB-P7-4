from flask import Flask, render_template
from comprehensive_analysis import analyze_all_emails

app = Flask(__name__)

@app.route("/")
def index():
    analysis_results = analyze_all_emails()
    return render_template("Test2.html", analysis_results=analysis_results)

if __name__ == "__main__":
    app.run(debug=True)