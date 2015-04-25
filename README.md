## RCPT
### MODULES
#### rcpt\_bounds
Given an image, will determine if the image is of a receipt.  If it is a receipt it will return the bounding polygon (trapezoid) of the receipt.
#### textline\_bounds
Given an image of a receipt with a bounding polygon, will return an array of bounding boxes for the text lines (all within the bounding polygon of the receipt)
#### read\_textlines
Given an image of a receipt with bounding boxes of the text lines, will run an OCR library to read and output each line as text
#### ner
Uses a named entity recognition library / technique (markov chains?) to output a JSON object corresponding to the pertinent information from the receipt.  For example, it would output the total string, items strings, price strings, etc.
#### fix\_chars
Given the JSON object corresponding to the items in a receipt, run a logistic regression for each price / number character to determine the probablity that the character is incorrect.  If the probability is high enough, replace that character. 
```
[PSEUDO CODE]
INPUT: "S4I5"
['S', '4', 'I', '5']
<prob highest that 'S' is '$'>
<prob highest that '4' is 4>
<prob highest that 'I' is 1>
<prob highest that '5' is 5>
OUTPUT: "$415"
```

remember that various probabilities need to be computed, because B -> 13 (so not just probaility of 0-9, $, but also probability that it's 13 (B), 17 (D), 12 (R), etc, or various other numbers which could be read as single characters.

#### clean
Given the 'correct' JSON object with characters that are what they should be, output the text in the correct format, to be used by the front end application.

Clean should store the output into a database.  There will be a unique id corresponding to image, and a unique id for each field / value which can be corrected by the user.  The raw string (from read\_textlines) should be included so that the user can help train the named entity recognition library.

```
INPUT: { ... { price: "$415" } }
OUTPUT: { ... { price: { value: 4.15, id: 'vo1c65' } }
```

#### user\_submit\_error
Users will be able to specify that they spotted an error.  Once they correct the error (which will be necessary for them to correctly use the application), this correction will be sent as a json object to the server.  This error will be added to an error queue.  The error queue will be run at some interval (ie, once a day) to train the pertinent algorithm (ner, fix\_chars, etc.).

```
error for count
{
  id: "<some random string>",
  correction: 4
}

error for string
{
  id: "<some random string>",
  correction: "chicken fingers"
}

error for price
{
  id: "<some random string>",
  correction: 4.16
}

etc
```

#### train
given a database of errors / receipt data, train all the various algorithms.

### TECHNOLOGY
DB: mongo to store all data.  Once all the kinks have been ironed out, will switch to postgresql
OCR: tesseract
CV: OpenCV, C++ native library for speed
Machine Learning: (Library) ?? TBD ?? Needed for NES, Logistic Regression, possible other applications
App: Whatever is the fastest / most convenient.  Possibly Java.  Would need JSON serialization libraries, probably wouldnt want it to be too hard to work with, REST libaries, HTTP server, etc.

Everything will be performed via a REST API.
```
POST /receipt/
```
will return
```
{
  id: "09ssdfgjk30jda9012jsadfjashd9",
  total: 23.15
  raw: "Burger good\n241 Hemingway \n Free delivery!\n chicken finger 1 S402\n2315 TOTAL"
  items: [ 
    {
      id: "ac1lgf",
      text: "chicken finger",
      price: 4.02,
      count: 1,
      raw: "chicken finger 1 S402"
    }
  ]
}
```

The raw strings are included so that training the algorithms can be done in a straightforward manner.  For example, a user won't be able to change the text.  They will only be able to select the correct text.
