#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Ошибка открытия видео!" << std::endl;
        return -1;
    }

    cv::Mat frame, gray, prevGray, diff, blurred, thresh;
    cv::Ptr<cv::BackgroundSubtractor> bgSubtractor = cv::createBackgroundSubtractorMOG2();

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // Преобразуем кадр в градации серого и размываем
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(gray, blurred, cv::Size(3, 3), 0);

        // Применяем фоновую модель
        bgSubtractor->apply(blurred, diff);

        // Устранение шумов через morph
        cv::morphologyEx(diff, diff, cv::MORPH_CLOSE, cv::Mat(), cv::Point(-1, -1), 2);

        // Применяем пороговую обработку
        cv::threshold(diff, thresh, 25, 255, cv::THRESH_BINARY);

        // Находим контуры и обводим большие объекты
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        for (const auto & contour : contours) {
            double area = cv::contourArea(contour);
            if (area > 450 && area < 5000) {  // Фильтрация по площади
                cv::Rect boundingBox = cv::boundingRect(contour);
                cv::rectangle(frame, boundingBox, cv::Scalar(0, 255, 0), 2);
            }
        }

        // Отображаем результат
        cv::imshow("Motion Detection", frame);

        if (cv::waitKey(30) == 'q') break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
