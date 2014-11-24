#create a new repository on the command line
touch README.md
git init
git add README.md
git config --global user.email $MAILNAME 
git config --global user.name $USERNAME 

git commit -m "first commit"
git remote add origin https://github.com/SharedSmally/poco.git
git push -u origin master


## push to remote repository: username/password
git push 

## pull files from remote repository
git pull

################################
git remote set-url origin https://name:password@github.org/repo.git 

#################################
vi .git/config:
https://username:password@github.com/WEMP/project-slideshow.git

####################### git commands
#Getting and Creating Projects under $homedir
git init  
git clone

#Basic Snapshotting
git add
git rm, mv
git diff
git commit
git reset

git status
git stash
git log

#Sharing and Updating Projects
git fetch, pull
git push
git remote

########################################################
#work with remote repos: pull->add/rm/mv->commit->push
########################################################

#Branching and Merging
git branch
git checkout
git merge
git log
git tag



