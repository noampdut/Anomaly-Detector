cmake_minimum_required(VERSION 3.20)
project(Anomaly_Detector)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(Anomaly_Detector MainTrain.cpp
        timeseries.cpp
        timeseries.h
        SimpleAnomalyDetector.cpp
        SimpleAnomalyDetector.h
        anomaly_detection_util.cpp
        anomaly_detection_util.h
        minCircle.cpp
        minCircle.h
        HybridAnomalyDetector.h
        HybridAnomalyDetector.cpp
        CLI.cpp
        CLI.h
        commands.h
        Server.cpp
        Server.h
        input.txt
        expected_output.txt
        expected_output_menu.txt
        )
