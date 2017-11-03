#pragma once
#include <string>
#include <iostream>
#include "ortools/base/join.h"
#include "XCSP3PrintCallbacks.h"
#include "BMFileParser.h"
#include "ortools/constraint_solver/constraint_solveri.h"
#include "ortools/base/logging.h"
#include "ortools/util/tuple_set.h"
#include <windows.h>
#include <io.h> 
using namespace cudacp;
using namespace std;
using namespace operations_research;


const int64 time_limit = 400000;
const string XPath = "BMPath.xml";
const string bmp_root = "E:\\Projects\\benchmarks\\xcsp\\";
const string bmp_ext = ".xml";
void getFilesAll(string path, vector<string>& files);

int main(const int argc, char ** argv) {

	if (argc <= 1) {
		std::cout << "no argument" << endl;
		return 0;
	}

	vector<string> files;
	getFilesAll(bmp_root + argv[1], files);

	int64 solve_time = 0;
	int64 num_solve = 0;
	int64 num_nosolution = 0;
	int64 num_node = 0;

	for (const auto f : files) {

		cout << f << endl;

		HModel *hm = new HModel();
		GetHModel(f, hm);
		Solver s("CPSimple");
		vector<IntVar*> vars(hm->vars.size());

		for (auto v : hm->vars)
			vars[v->id] = s.MakeIntVar(v->vals, v->name);

		for (auto t : hm->tabs) {
			vector<IntVar*> vs;
			for (auto v : t->scope)
				vs.push_back(vars[v->id]);
			IntTupleSet ts(t->scope.size());
			ts.InsertAll(t->tuples);
			s.AddConstraint(s.MakeAllowedAssignments(vs, ts));
			ts.Clear();
		}

		DecisionBuilder* const db = s.MakePhase(vars, Solver::CHOOSE_MIN_SIZE, Solver::ASSIGN_MIN_VALUE);
		SearchLimit* limit = s.MakeTimeLimit(time_limit);
		s.NewSearch(db, limit);

		if (s.NextSolution()) {
			solve_time += s.wall_time();
			num_node += s.branches();
			++num_solve;
		}
		else {
			if (s.wall_time() < s.GetTime(limit)) {
				//no solution
				solve_time += s.wall_time();
				num_node += s.branches();
				++num_solve;
				++num_nosolution;
			}
		}

		s.EndSearch();
		delete hm;
	}

	std::cout << "---------------------------------------------------------------------" << endl;
	std::cout << "num_solved = " << num_solve << " || no solutions = " << num_nosolution << " || nodes = " << num_node << " || sum time = " << solve_time << endl;

	return 0;
};

void getFilesAll(string path, vector<string>& files) {
	//文件句柄 
	intptr_t  hFile = 0;
	//文件信息 
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
		do {
			if ((fileinfo.attrib & _A_SUBDIR)) {
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
					getFilesAll(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else {
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}