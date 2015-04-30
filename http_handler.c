#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/curl_easy.h"

#define POSTURL "10.7.0.15/acs_dev"
#define POSTFIELDS_INFORM "<?xml version=\"1.0\" encoding=\"UTF-8\"?> <SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:SOAP-ENC=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:ns1=\"urn:dslforum-org:cwmp-1-2\" xmlns:ns2=\"urn:dslforum-org:cwmp-1-1\" xmlns:ns3=\"urn:dslforum-org:cwmp-1-0\"><SOAP-ENV:Body SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><ns1:Inform><DeviceId><Manufacturer>Sunwave</Manufacturer><OUI>FFFFFF</OUI><ProductClass>eNodeB</ProductClass><SerialNumber>80000002</SerialNumber></DeviceId><Event xsi:type=\"SOAP-ENC:Array\" SOAP-ENC:arrayType=\"ns1:EventStruct[1]\"><EventStruct><EventCode>1 BOOT</EventCode><CommandKey></CommandKey></EventStruct></Event><MaxEnvelopes>1</MaxEnvelopes><CurrentTime>2015-04-01T03:25:38Z</CurrentTime><RetryCount>0</RetryCount><ParameterList xsi:type=\"SOAP-ENC:Array\" SOAP-ENC:arrayType=\"ns1:ParameterValueStruct[4]\"><ParameterValueStruct><Name>InternetGatewayDevice.Services.FAPService.1.FAPControl.OpState</Name><Value>0</Value></ParameterValueStruct><ParameterValueStruct><Name>InternetGatewayDevice.Services.FAPService.1.FAPControl.RFTxStatus</Name><Value>0</Value></ParameterValueStruct><ParameterValueStruct><Name>InternetGatewayDevice.Services.FAPService.1.REM.LTE.LastScanTime</Name><Value>0001-01-01T00:00:00Z</Value></ParameterValueStruct><ParameterValueStruct><Name>InternetGatewayDevice.Services.FAPService.1.CellConfig.LTE.RAN.NeighborListInUse.X_VENDOR_LastNRTUpdateTime</Name><Value>0001-01-01T00:00:00Z</Value></ParameterValueStruct></ParameterList></ns1:Inform></SOAP-ENV:Body></SOAP-ENV:Envelope>"

int http_comm_with_acs(char *url, char *xml_send, char **xml_recv)
{
	return curl_easy_send_recv(url, xml_send, xml_recv);
}

void main()
{
	char *resp;
	http_comm_with_acs(POSTURL, POSTFIELDS_INFORM, &resp);
	printf("resp: %s\n", resp);
	free(resp);
}
