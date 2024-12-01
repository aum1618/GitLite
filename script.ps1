# Script to merge changes from 'durrani' to all other branches and push the updates
$branches = git branch | ForEach-Object { $_.Trim() } # Get all branches
$currentBranch = (git branch --show-current).Trim() # Get the current branch
$sourceBranch = "umer" # Source branch for merging

if ($currentBranch -ne $sourceBranch) {
    Write-Output "Switching to $sourceBranch branch..."
    git checkout $sourceBranch
}

foreach ($branch in $branches) {
    if ($branch -ne $sourceBranch) {
        Write-Output "Merging changes from $sourceBranch into $branch..."
        
        # Checkout the target branch
        git checkout $branch

        # Merge changes from the source branch
        git merge $sourceBranch

        if ($LASTEXITCODE -eq 0) {
            Write-Output "Pushing $branch to remote repository..."
            git push origin $branch
        }
        else {
            Write-Error "Merge failed for $branch. Resolve conflicts manually."
            exit 1
        }
    }
}

# Return to the original branch
Write-Output "Switching back to the original branch: $currentBranch..."
git checkout $currentBranch

Write-Output "All branches updated successfully."
