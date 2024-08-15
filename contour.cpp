#include <opencv2/opencv.hpp>
using namespace cv;

// 用于填充矩形的边距
const int PADDING = 10;

// 扩展矩形以添加边距
Rect extendRect(const Rect& rect, int padding) {
    return Rect(
        rect.x - padding,
        rect.y - padding,
        rect.width + 2 * padding,
        rect.height + 2 * padding
    );
}

int main(int argc, char** argv) {
    // 定义图像的宽度和高度
    int width = 800;
    int height = 600;

    // 创建一个全黑图像，使用CV_8UC3表示三通道的图像（例如RGB图像）
    Mat blackImage(height, width, CV_8UC3, Scalar(0, 0, 0));

    // 检查输入参数数量
    if (argc != 3) {
        std::cerr << "Usage: ./contour <input_image> <output_image>" << std::endl;
        return -1;
    }

    // 读取输入图像并转换为灰度图像
    Mat src = imread(argv[1], IMREAD_GRAYSCALE);
    if (src.empty()) {
        std::cerr << "Error: Cannot load image!" << std::endl;
        return -1;
    }

    // 将图像二值化
    Mat binary;
    threshold(src, binary, 128, 255, THRESH_BINARY);

    // 应用开运算
    Mat opened;
    Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
    morphologyEx(binary, opened, MORPH_OPEN, kernel);

    // 检测轮廓
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    findContours(opened, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    // 创建输出图像
    Mat output = src.clone(); // 克隆原图像以进行绘制

    // 存储所有合并后的矩形
    std::vector<Rect> extendedRects;

    for (size_t i = 0; i < contours.size(); i++) {
        // 获取轮廓的外接矩形
        Rect boundingRect = cv::boundingRect(contours[i]);
        float aspectRatio = (float)boundingRect.width / boundingRect.height;

        // 输出长宽比
       // std::cout << "Contour " << i << " Aspect Ratio: " << aspectRatio << std::endl;
       
            // 在图像上绘制长宽比
            std::string aspectRatioText = "AR: " + std::to_string(aspectRatio);
            Point textOrg(boundingRect.x, boundingRect.y - 10); // 文本位置
            putText(output, aspectRatioText, textOrg, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1, LINE_AA);

        // 设置长宽比阈值
        float minAspectRatio = 0.16; // 你可以根据需要调整这些值
        float maxAspectRatio = 0.22;

        // 根据长宽比掩盖小轮廓
        if (aspectRatio >= minAspectRatio && aspectRatio <= maxAspectRatio) {
            // 扩展矩形
            Rect extendedRect = extendRect(boundingRect, PADDING);

            // 检查是否有重叠
            bool overlap = false;
            for (const auto& existingRect : extendedRects) {
                if ((existingRect & extendedRect).area() > 0) {
                    overlap = true;
                    break;
                }
            }

            // 如果没有重叠，则添加到列表中
            if (!overlap) {
                extendedRects.push_back(extendedRect);
            }

            // 在轮廓上绘制矩形框
            rectangle(blackImage, boundingRect, Scalar(255, 255, 255), 2); // 白色矩形框
        }
    }

    // 合并所有扩展后的矩形
    Rect mergedRect;
    if (!extendedRects.empty()) {
        mergedRect = extendedRects[0];
        for (size_t i = 1; i < extendedRects.size(); i++) {
            mergedRect |= extendedRects[i];
        }

        // 绘制合并后的矩形
        rectangle(blackImage, mergedRect, Scalar(0, 0, 255), 1); // 红色矩形框
    }

    // 显示图像
    imshow("find Image", blackImage);
    imshow("Processed Image", output);

    // 保存绘制后的轮廓图像
    imwrite(argv[2], output); // 保存处理后的图像

    waitKey(0); // 等待用户按键后关闭窗口
    return 0;
}
