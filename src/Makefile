CXXFLAGS = -DDEBUG -g -pipe -ansi -Wall

all: crafty

NLog.o: NLog.cpp NLog.h
	${CXX} ${CXXFLAGS} -c NLog.cpp  `wx-config --cxxflags`

rawsocket.o: rawsocket.h
	${CXX} ${CXXFLAGS} -c rawsocket.cpp

packet.o: packet.cpp packet.h
	${CXX} ${CXXFLAGS} -c packet.cpp `wx-config --cxxflags`
	
tcp_packet.o: tcp_packet.cpp tcp_packet.h ipv4_packet.h packet.h
	${CXX} ${CXXFLAGS} -c tcp_packet.cpp

ipv4_packet.o: ipv4_packet.cpp ipv4_packet.h packet.h
	${CXX} ${CXXFLAGS} -c ipv4_packet.cpp
	
hex_edit.o: hex_edit.cpp hex_edit.h
	${CXX} ${CXXFLAGS} -c hex_edit.cpp `wx-config --cxxflags`

craft_wdr.o: craft_wdr.cpp craft_wdr.h
	$(CXX) ${CXXFLAGS} -c craft_wdr.cpp `wx-config --cxxflags`

#main.o: main.cpp ipv4_packet.h
#	${CXX} ${CXXFLAGS} -c main.cpp
	
craft.o: craft.cpp craft.h hex_edit.h craft_wdr.h
	$(CXX) ${CXXFLAGS} -c craft.cpp `wx-config --cxxflags`

crafty : craft.o packet.o NLog.o tcp_packet.o ipv4_packet.o craft_wdr.o hex_edit.o
	${CXX} ${CXXFLAGS} -o crafty NLog.o craft.o craft_wdr.o hex_edit.o packet.o tcp_packet.o ipv4_packet.o -lnet `wx-config --libs`

clean :
	rm -rf *.o crafty
