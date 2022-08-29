

## 拉取仓库
git clone [url]

## 提交到本地仓库
git add .
git commit -m "[message]"

## 推送到远程仓库
git push origin master

## 本地代码与远程同步
git pull

## 回退到某个版本：（回退的代码都会被删掉）
git reset --hard fa6649b2524c683c2acb54f83dcea7e694380c07
git push -f origin [branchName] 同步到远端仓库

## 修改最新的一次commit信息
git commit --amend

## 删除本地多余的分支
git remote prune origin

