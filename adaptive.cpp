#include <opencv2/opencv.hpp>
using namespace cv;

int main(int argc, char** argv) {
    //传入参数（argc表示有三个参数 根据if条件写出需要什么）
    if (argc != 3) {
        std::cerr << "Usage: ./threshold <input_image> <output_image>" << std::endl;
        return -1;
    }

    // 读取输入图像并转换为灰度图像
    Mat src = imread(argv[1], IMREAD_GRAYSCALE);
    //直接使用原彩色图像进行腐蚀膨胀等
    Mat src1=imread(argv[1], IMREAD_COLOR);
    if (src.empty()) {
        std::cerr << "Error: Could not open or find the image." << std::endl;
        return -1;
    }
   //前面部分各类均相同
    Mat dst;//表示目标文件，需创建
    //下面为各不同功能函数，根据函数原型参数需要设置即可
    //最后imshow或再加imwrite保存即可

    // 应用自适应阈值处理
    adaptiveThreshold(src, dst, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 11, 2);

    // 显示自适应阈值处理后的图像
   // imshow("Adaptive Threshold Image", dst);

    // 定义卷积核 (3x3 均值滤波器)
    float kernel_data[9] = {1.0/9, 1.0/9, 1.0/9, 
                            1.0/9, 1.0/9, 1.0/9, 
                            1.0/9, 1.0/9, 1.0/9};
    Mat kernel(3, 3, CV_32F, kernel_data);

    // 应用2D卷积（图像过滤）
    Mat filtered_dst;
    filter2D(dst, filtered_dst, -1, kernel);

    // 显示2D卷积处理后的图像
    //imshow("Filtered Image", filtered_dst);

      Mat dst1;

    // 应用高斯模糊
    GaussianBlur(src, dst1, Size(15, 15), 0);

    //显示模糊后的图像
    //imshow("Gaussian Blurred Image", dst1);

    //应用腐蚀
    Mat dst2;
    Mat kernel2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 50));
    erode(src, dst2, kernel2, cv::Point(-1, -1), 1);

    //imshow("erode Image", dst2);
    //膨胀
   Mat dilatedImage;
    dilate(src, dilatedImage, kernel, Point(-1, -1), 30);

    //imshow("Dilated Image", dilatedImage);
    //开运算
    Mat dst3;
    morphologyEx(src1, dst3, cv::MORPH_OPEN, kernel, cv::Point(-1, -1), 3);
    // imshow("morpho Image", dst3);

//sobel算子
    cv::Mat grad_x, grad_y;
cv::Mat grad;
int ddepth = CV_16S;//与其他前面不同之处
cv::Sobel(src1, grad_x, ddepth, 1, 0, 3); // 计算 x 方向的梯度
cv::Sobel(src1, grad_y, ddepth, 0, 1, 3); // 计算 y 方向的梯度
cv::convertScaleAbs(grad_x, grad_x); // 转换为 8 位图像
cv::convertScaleAbs(grad_y, grad_y); // 转换为 8 位图像
cv::addWeighted(grad_x, 0.5, grad_y, 0.5, 0, grad); // 合并 x 和 y 方向的梯度
 //imshow("grad Image", grad);

Mat canny_dst;
 double lower_threshold = 50; // 低阈值
    double upper_threshold = 150; // 高阈值
    Canny(src1, canny_dst, lower_threshold, upper_threshold);
 imshow("canny Image", canny_dst);
     //保存变换后图像
    imwrite(argv[2], canny_dst);
    waitKey(0); // 等待用户按键后关闭窗口
    return 0;
}
