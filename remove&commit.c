int git_remove(void *repo, const char *file_name[], int file_num, bool bKeepLocal, gitlocalIndexcallback localcallback, void *payload)
{
	git_strarray _pathspecs = {0};
	char arr[MAX_PATH]={0}; 
	git_index *index = NULL;
	char **strArray = NULL;
	stu_local_cb *local_opt = NULL;
	int error = 0;

	const char *repo_path = repo_path_cb((git_repository *)repo);
	if(!repo_path)
	{
		return -1;
	}

	do  /* try catch */
	{
		_pathspecs.strings = strArray;
		_pathspecs.count = file_num;

		error = git_repository_index(&index, (git_repository *)repo);
		if(error < 0)
		{
			break;
		}
    
		error = git_index_remove_all(index, &_pathspecs, NULL, NULL);  //index_matched_path_cb
		if(error < 0)
		{
			break;
		}

		error = git_index_write(index);  // write to the dask? state is deleted
		if(error < 0)
		{
			break;
		}

	}while(0);
	git_index_free(index);
	return error;
}

int commit(void *repo, const char *file_name[], const unsigned int fileNum, const char *message, gitlocalIndexcallback localcallback, void *payload)
{
	int error = -1;
	git_signature *sig = NULL;
	git_oid git;
	git_oid commit_id;
	void* pIndex = NULL;
	void* tree = NULL;
	git_reference* ref_head = NULL;
	size_t parent_count = 0;
	git_commit* parent_commit = NULL;
	const git_commit *parents[] = { NULL };
	char arr[MAX_PATH]={0};
	git_strarray pathspec = {0};
	git_strarray mergePath = {0};
	char **strArray = NULL;
	char **strMergeFileArray = NULL;
	stu_local_cb *local_opt = NULL;
	int nFileCount = 0;
	int nMergeFileCount = 0;
	git_commit* their_commit = NULL;
	git_commit* our_commit = NULL;
	git_reference* origin_master = NULL;
	git_reference* local_master = NULL;
	const git_annotated_commit* their_head[10] = {0};
	git_merge_options merge_opt = GIT_MERGE_OPTIONS_INIT;
	merge_opt.file_favor = GIT_MERGE_FILE_FAVOR_UNION;
	git_checkout_options checkout_opt = GIT_CHECKOUT_OPTIONS_INIT;
	checkout_opt.checkout_strategy = GIT_CHECKOUT_SAFE;
	git_reference *branch = NULL;
	unsigned int flags = 0;


	if (NULL == commit_cb || NULL == signature_default_cb || NULL == write_tree_cb || 
		NULL == lookup_tree_cb || NULL == repo_head_cb || NULL == reference_target_cb || 
		NULL == repo_path_cb || NULL == repo)
	{
		return error;
	}

	do  /* try catch */
	{
		const char *repo_path = git_repository_path((git_repository*)repo);
		if(!repo_path)
		{
			break;
		}

		error = git_signature_default(&sig, (git_repository*)repo);
		if (error < 0)
		{
			break;
		}

		error = git_repository_head(&ref_head,(git_repository*)repo);
		if (error == GIT_OK)
		{
			error = git_commit_lookup(&parent_commit, (git_repository*)repo, reference_target_cb(ref_head));
			if (error < 0) 
			{
				break;
			}

			parent_count = 1;
			parents[0] = parent_commit;
			git_reference_free(ref_head);
		}
		else if (error != GIT_EUNBORNBRANCH)
		{
			break;
		}

		error = git_repository_index((git_index**)&pIndex, (git_repository*)repo);
		if (error < 0)
		{
			break;
		}

		for(int i = 0; i < nFileCount; ++i)
		{
			error = git_status_file(&flags, (git_repository *)repo, vecPath[i].c_str());
			if(GIT_EAMBIGUOUS == error || GIT_ENOTFOUND == error)
			{
				continue;
			}

			if( GIT_STATUS_INDEX_DELETED & flags || GIT_STATUS_WT_DELETED & flags )
			{
				error = git_index_remove_bypath((git_index *)pIndex, file_name[i]);  
				if(error < 0)
				{
					break;
				}
			}
			else
			{
				error = git_index_add_bypath((git_index *)pIndex, file_name[i]);
				if(error < 0)
				{
					break;
				}
			}

			error = git_index_write((git_index*)pIndex);    // write dask
			if (error < 0) 
			{
				break;
			}

		}
	  
		error = git_index_write_tree(&git, (git_index*)pIndex);   // write to tree ?
		if (error < 0) 
		{
			break;
		}

		error = git_tree_lookup((git_tree**)&tree, (git_repository *)repo, &git);  // get the repository tree?
		if (error < 0) 
		{
			break;
		}

		error = git_commit_create(&commit_id, (git_repository*)repo, "HEAD", sig, sig, NULL, message, (git_tree *)tree, 1, parents);  // default commit remove file
		if(error < 0)
		{
			break;
		}	
	}while(0);
	
}


















