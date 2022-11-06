# ExploringGRPC

Exploring GRPC communication framework.</br>
This repo has a toy example on how to use protocol buffers and grpc to exchange data between a client and a server.</br>
I created a python server and a C++ client. The idea is to transfer stock data between client and server.</br>
There are four methods involved in this communication:</br>
- Simple, bi-directional information exchange, in which the client sends the Ticker, the stock property (price at open, close, high, low, etc.) and the required time, and the server returns the price for that stock.</br>
- Bi-directional information exchange with different, optional data types. Here I demonstrate the usage of the OneOf protocol buffer message type. The client sends a Ticker and the time, the server returns all prices for that stock (open, close, high, low, etc.).</br>
- Server stream. The client sends a Ticker and the required property (price at open, close, high, low, etc.), and the server returns the price at time stamps from the database (in this case, a CSV file).</br>
- Client stream. The client streams the Ticker, time and volume of different stocks to the server. In this case, I also created a (very) simple thread manager, and the data for each stock is sent in a separate thread.</br>
