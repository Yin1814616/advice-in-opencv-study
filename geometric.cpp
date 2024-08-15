#include <stdio.h>
#include <opencv2/opencv.hpp>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: mask <src_path>\n");
        return -1;
    }

    // 读取图像
    cv::Mat src = cv::imread(argv[1], cv::IMREAD_COLOR);
    if (src.empty()) {
        std::cerr << "Error: Cannot open image file." << std::endl;
        return -1;
    }

    // 显示原始图像
    cv::imshow("Original Image", src);

    // 仿射变换
    // 1. 定义仿射变换矩阵
    cv::Point2f srcTri[3];
    cv::Point2f dstTri[3];

    // 设置源图像中三点的位置
    srcTri[0] = cv::Point2f(0, 0);
    srcTri[1] = cv::Point2f(src.cols - 1.f, 0);
    srcTri[2] = cv::Point2f(0, src.rows - 1.f);

    // 设置目标图像中三点的位置
    dstTri[0] = cv::Point2f(src.cols*0.0f, src.rows*0.33f);
    dstTri[1] = cv::Point2f(src.cols*0.85f, src.rows*0.25f);
    dstTri[2] = cv::Point2f(src.cols*0.15f, src.rows*0.7f);

    // 获取仿射变换矩阵
    cv::Mat warp_mat = cv::getAffineTransform(srcTri, dstTri);

    // 仿射变换输出
    cv::Mat warp_dst = cv::Mat::zeros(src.rows, src.cols, src.type());
    cv::warpAffine(src, warp_dst, warp_mat, warp_dst.size());

    // 显示仿射变换后的图像
    cv::imshow("Affine Transform", warp_dst);

    // 透视变换
    // 1. 定义透视变换矩阵
    cv::Point2f srcQuad[4];
    cv::Point2f dstQuad[4];

    // 设置源图像中四点的位置
    srcQuad[0] = cv::Point2f(0, 0);
    srcQuad[1] = cv::Point2f(src.cols - 1.f, 0);
    srcQuad[2] = cv::Point2f(src.cols - 1.f, src.rows - 1.f);
    srcQuad[3] = cv::Point2f(0, src.rows - 1.f);

    // 设置目标图像中四点的位置
    dstQuad[0] = cv::Point2f(src.cols*0.0f, src.rows*0.33f);
    dstQuad[1] = cv::Point2f(src.cols*0.85f, src.rows*0.25f);
    dstQuad[2] = cv::Point2f(src.cols*0.9f, src.rows*0.9f);
    dstQuad[3] = cv::Point2f(src.cols*0.2f, src.rows*0.7f);

    // 获取透视变换矩阵
    cv::Mat perspective_mat = cv::getPerspectiveTransform(srcQuad, dstQuad);

    // 透视变换输出
    cv::Mat perspective_dst = cv::Mat::zeros(src.rows, src.cols, src.type());
    cv::warpPerspective(src, perspective_dst, perspective_mat, perspective_dst.size());

    // 显示透视变换后的图像
    cv::imshow("Perspective Transform", perspective_dst);

    // 等待按键并退出
    cv::waitKey(0);
    return 0;
}
