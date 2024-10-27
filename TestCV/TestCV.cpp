#include </home/sergey/opencv/include/opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Открываем первую камеру
    cv::VideoCapture cap('/home/sergey/Code/ProjectsC++/video/VideoANsys.mp4');
    if (!cap.isOpened()) {
        std::cerr << "Ошибка открытия видео!" << std::endl;
        return -1;
    }

    cv::Mat frame, gray, prevGray, diff, blurred, thresh;

    while (true) {
        // Читаем текущий кадр
        cap >> frame;
        if (frame.empty()) break;

        // Преобразуем кадр в оттенки серого
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        // Применяем размытие для уменьшения шума
        cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);

        if (!prevGray.empty()) {
            // Вычисляем разницу между текущим и предыдущим кадром
            cv::absdiff(prevGray, blurred, diff);

            // Применяем адаптивную пороговую обработку
            cv::adaptiveThreshold(
                diff, thresh, 255, 
                cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 2
            );

            // Применяем морфологические операции для удаления мелких шумов
            cv::erode(thresh, thresh, cv::Mat(), cv::Point(-1, -1), 2);
            cv::dilate(thresh, thresh, cv::Mat(), cv::Point(-1, -1), 2);

            // Отображаем результат
            cv::imshow("Motion Detection", thresh);
        }

        // Сохраняем текущий кадр как предыдущий для следующей итерации
        blurred.copyTo(prevGray);

        // Ожидаем нажатия клавиши 'q' для выхода
        if (cv::waitKey(30) == 'q') break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
