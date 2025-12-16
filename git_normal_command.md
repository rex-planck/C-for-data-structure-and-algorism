# Git 常用命令备忘录

## 1. 核心三板斧 (日常上传)
- `git add .`  
  **收集**：把所有修改加入暂存区。
- `git commit -m "备注信息"`  
  **存档**：保存并添加备注。
- `git push`  
  **发送**：推送到 GitHub 云端。

## 2. 常用设置 (一次性)
- `git remote add origin <网址>` : 关联远程仓库
- `git branch -M main` : 重命名分支为 main

## 3. 救火命令
- `git pull origin main` : 把云端的更新拉取到本地 (解决冲突)
- `git rm --cached *.exe` : 停止追踪某类文件 (不删本地)
- `git status` : 查看当前状态 (不知道发生了什么时用)
