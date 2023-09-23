from flask import Flask, render_template, request, jsonify
import pickle

app = Flask(__name__)

# Load the machine learning model
with open('room.pkl', 'rb') as f:
    model = pickle.load(f)

# Define the index route
@app.route('/')
def index():
    # Render the index.html template
    return render_template('index.html')

# Define the predict route
@app.route('/predict', methods=['POST'])
def predict():
    input_data = request.get_json()
    input1 = float(input_data['input1'])
    input2 = float(input_data['input2'])
    input3 = float(input_data['input3'])
    input4 = float(input_data['input4'])

    prediction = model.predict([[input1, input2, input3, input4]])[0]

    if prediction == 1:  # Assuming 1 indicates the room is occupied
        result = "Room is occupied"
    else:
        result = "Room isn't occupied"

    return jsonify({'prediction': result})


if __name__ == '__main__':
    app.run(debug=True)