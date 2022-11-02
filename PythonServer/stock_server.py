import sys
sys.path.append("proto_files")
import grpc
from concurrent import futures
import pandas as pd
import stock_messages_pb2
import stock_service_pb2_grpc

PORT = '54321'

class StockData(object):
    def __init__(self):
        self.data = pd.read_csv('data/MarkePrice.csv')


class StockServer(StockData, stock_service_pb2_grpc.StockServiceServicer):
    
    def StartServer(self):
        server = grpc.server(futures.ThreadPoolExecutor(max_workers=5))
        stock_service_pb2_grpc.add_StockServiceServicer_to_server(self, server)
        server.add_insecure_port('[::]:'+PORT)
        print('Starting stock server at port {} ...'.format(PORT))
        server.start()
        server.wait_for_termination()

    def GetStockValueAtTime(self, request, context):
        company_name = request.stock_data.company_name.name
        stock_property = request.stock_data.property
        time_stamp = request.time_stamp
        self.data.loc[(self.data['Date']=='2022-01-03') & (self.data['Ticker']=='CVX')]['LOW']



    def GetStockValuesAtTime(self, request, context):
        company_name = company_name.name
        time_stamp = request.time_stamp
        stock_values_dict = self.data
        for key in stock_values_dict.keys():
            return stock_messages_pb2.StockValues.StockByProperty()

    def GetStockValueAtAllTimes(self, request, context):
        company_name = request.stock_data.company_name.name
        stock_property = request.stock_data.property
        for value in stock_values:
            yield stock_messages_pb2.StockValue(value=value)

    def SendStockVolume(self, request_iterator, context):
        for request in request_iterator:
            company_name = request.company_name.name
            stock_volume = request.stock_volume.volume
            time_stamp = request.time_stamp
            print('At {0}, the volume of stock {1} was {2}'.format(time_stamp, company_name, stock_volume))




if __name__ == '__main__':
    serv = StockServer()
    serv.StartServer()