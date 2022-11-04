import sys
sys.path.append("proto_files")
import grpc
from concurrent import futures
import pandas as pd
import stock_messages_pb2 as s
import stock_service_pb2_grpc

PORT = '54321'

# Headers
DATE = 'Date'
TICKER = 'Ticker'
OPEN = 'OPEN'
HIGH = 'HIGH'
LOW = 'LOW'
CLOSE = 'CLOSE'
ADJ_CLOSE = 'ADJ_CLOSE'

class StockData(object):
    def __init__(self):
        self.data = pd.read_csv('data/MarkePrice.csv')


class StockServer(StockData, stock_service_pb2_grpc.StockServiceServicer):
    
    def StartServer(self):

        ''' Server starts and waits for method calls from client '''

        server = grpc.server(futures.ThreadPoolExecutor(max_workers=5))
        stock_service_pb2_grpc.add_StockServiceServicer_to_server(self, server)
        server.add_insecure_port('[::]:'+PORT)
        print('Starting stock server at port {} ...'.format(PORT))
        server.start()
        server.wait_for_termination()

    def GetStockValueAtTime(self, request, context):

        ''' Sends stock price given a ticker, a time stamp and the required property '''

        ticker = request.stock_data.company_name.name
        stock_property = request.stock_data.property
        timestamp = request.timestamp
        value = self.data.loc[(self.data[DATE]==timestamp) & (self.data[TICKER]==ticker)]['LOW'].values[0]
        return s.StockValue(value=value)

    def GetStockValuesAtTime(self, request, context):

        ''' Sends stock price of all properties of a company at a given time '''

        ticker = request.company_name.name
        timestamp = request.timestamp
        property_list = [OPEN, HIGH, LOW, CLOSE, ADJ_CLOSE]
        val_list = self.data.loc[(self.data[DATE]==timestamp) & (self.data[TICKER]==ticker), property_list].values.flatten().tolist()
        values = []
        for property, value in zip(property_list, val_list):
            if property == OPEN: values.append(s.StockByProperty(value_open=s.StockValue(value=value)))
            if property == HIGH: values.append(s.StockByProperty(value_high=s.StockValue(value=value)))
            if property == LOW: values.append(s.StockByProperty(value_low=s.StockValue(value=value)))
            if property == CLOSE: values.append(s.StockByProperty(value_close=s.StockValue(value=value)))
            if property == ADJ_CLOSE: values.append(s.StockByProperty(value_adj_close=s.StockValue(value=value)))
        return s.StockValues(value_by_property=values)

    def GetStockValueAtAllTimes(self, request, context):

        '''' Streams all values for a given company ticker and required property '''

        ticker = request.company_name.name
        stock_property = s.StockProperty.Name(request.property)
        values = self.data.loc[self.data[TICKER]==ticker][stock_property].values.flatten().tolist()
        for value in values:
            yield s.StockValue(value=value)

    def SendStockVolume(self, request_iterator, context):

        ''' Gets volume from client and prints to console '''

        max_volume = 0
        max_timestamp = "01-01-2000"
        for request in request_iterator:
            company_name = request.company_name.name
            stock_volume = request.stock_volume.volume
            timestamp = request.timestamp
            if stock_volume >= max_volume:
                max_timestamp = timestamp
                max_volume = stock_volume
        print('\nThe maximum volume of stock {0} was {1} at {2}'.format(company_name, max_volume, max_timestamp))




if __name__ == '__main__':
    serv = StockServer()
    serv.StartServer()