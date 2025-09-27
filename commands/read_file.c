struct ReadFileData
{
	int line;
};
struct ReadFileData
create_read_file_data()
{
	struct ReadFileData data;
	data.line = -1;
	return data;
}

int read_file_build_command(optionKey);