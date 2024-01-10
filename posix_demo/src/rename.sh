#!/bin/bash

# 遍历当前目录下的所有.hpp文件
for file in *.cpp; do
    # 检查文件是否存在
    if [ -e "$file" ]; then
        # 将文件名从.hpp替换为.cpp
        new_name="${file%.cpp}.hpp"

        # 检查新文件名是否已存在
        if [ -e "$new_name" ]; then
            echo "文件 $new_name 已存在，跳过重命名。"
        else
            # 执行重命名
            mv "$file" "$new_name"
            echo "已将文件 $file 重命名为 $new_name。"
        fi
    fi
done

