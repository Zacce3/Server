from flask import Flask, request, redirect, url_for

app = Flask(__name__)

# Store submitted data in a list (in-memory data store)
submissions = []

# Home route with a form
@app.route("/", methods=["GET", "POST"])
def home():
    # Handle form submission
    if request.method == "POST":
        name = request.form.get("name")
        message = request.form.get("message")
        submissions.append({"name": name, "message": message})  # Save data
        return redirect(url_for("home"))  # Redirect to avoid form resubmission

    # Display the form and the table of submissions
    return f"""
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Window control interface</title>
        <style>
            body {{
                font-family: Arial, sans-serif;
                max-width: 800px;
                margin: auto;
                padding: 20px;
            }}
            h1 {{ color: #333; }}
            form {{
                margin-bottom: 20px;
            }}
            label, input, textarea {{
                display: block;
                margin: 5px 0;
            }}
            table {{
                width: 100%;
                border-collapse: collapse;
            }}
            th, td {{
                border: 1px solid #ddd;
                padding: 8px;
            }}
            th {{
                background-color: #f4f4f4;
            }}
        </style>
    </head>
    <body>
        <h1>Window control interface!</h1>
        <p>Data shown below:</p>

        <form method="post">
            <label for="name">Name:</label>
            <input type="text" id="name" name="name" required>

            <label for="message">Message:</label>
            <textarea id="message" name="message" required></textarea>

            <button type="submit">Submit</button>
        </form>

        <h2>Submitted Messages:</h2>
        <table>
            <tr>
                <th>Name</th>
                <th>Message</th>
            </tr>
            {" ".join([f"<tr><td>{entry['name']}</td><td>{entry['message']}</td></tr>" for entry in submissions])}
        </table>

    </body>
    </html>
    """

# About page with additional information
@app.route("/about")
def about():
    return """
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>About Page</title>
        <style>
            body {{
                font-family: Arial, sans-serif;
                max-width: 800px;
                margin: auto;
                padding: 20px;
            }}
            h1 {{ color: #333; }}
        </style>
    </head>
    <body>
        <h1>About This Website</h1>
        <p>This is a more complex version of a website built using Flask in a single Python file.</p>
        <p>It includes:</p>
        <ul>
            <li>A form to submit data</li>
            <li>In-memory storage of submitted data</li>
            <li>A styled table to display the data</li>
        </ul>
        <a href="/">Home</a>
    </body>
    </html>
    """

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)


