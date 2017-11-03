//#pragma once
//#include <string>
//#include <iostream>
//#include "ortools/base/join.h"
//#include "XCSP3PrintCallbacks.h"
//#include "BMFileParser.h"
//#include "ortools/constraint_solver/constraint_solveri.h"
//#include "ortools/base/logging.h"
//#include "ortools/util/tuple_set.h"
//#include <windows.h>
//#include <io.h> 
//using namespace cudacp;
//using namespace std;
//using namespace operations_research;
//
//
//const int64 time_limit = 400000;
//const string XPath = "BMPath.xml";
//const string bmp_root = "E:/Projects/benchmarks/xcsp/";
//const string bmp_ext = ".xml";
//void getFilesAll(string path, vector<string>& files);
//
//int main(const int argc, char ** argv) {
//
//	if (argc <= 1) {
//		std::cout << "no argument" << endl;
//		return 0;
//	}
//
//	vector<string> files;
//	getFilesAll(bmp_root + argv[1], files);
//
//	for (const auto f : files) {
//
//		cout << f << endl;
//
//		HModel *hm = new HModel();
//		GetHModel(f, hm);
//		Solver s("CPSimple");
//		vector<IntVar*> vars(hm->vars.size());
//
//		for (auto v : hm->vars)
//			vars[v->id] = s.MakeIntVar(v->vals, v->name);
//
//		for (auto t : hm->tabs) {
//			vector<IntVar*> vs;
//			for (auto v : t->scope)
//				vs.push_back(vars[v->id]);
//			IntTupleSet ts(t->scope.size());
//			ts.InsertAll(t->tuples);
//			s.AddConstraint(s.MakeAllowedAssignments(vs, ts));
//			ts.Clear();
//		}
//
//		DecisionBuilder* const db = s.MakePhase(vars, Solver::CHOOSE_MIN_SIZE, Solver::ASSIGN_MIN_VALUE);
//		SearchLimit* limit = s.MakeTimeLimit(time_limit);
//		s.NewSearch(db, limit);
//
//		if (s.NextSolution()) {
//			cout << "has solution";
//			cout << " || solve time = " << s.wall_time();
//			cout << " || nodes =" << s.branches();
//		}
//		else {
//			if (s.wall_time() < s.GetTime(limit)) {
//				cout << "no solution";
//				cout << " || solve time = " << s.wall_time();
//				cout << " || nodes =" << s.branches();
//			}
//		}
//
//		s.EndSearch();
//		cout << endl;
//		delete hm;
//	}
//	return 0;
//};
//
//void getFilesAll(string path, vector<string>& files) {
//	//文件句柄 
//	intptr_t  hFile = 0;
//	//文件信息 
//	struct _finddata_t fileinfo;
//	string p;
//	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
//		do {
//			if ((fileinfo.attrib & _A_SUBDIR)) {
//				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
//					//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
//					getFilesAll(p.assign(path).append("\\").append(fileinfo.name), files);
//				}
//			}
//			else {
//				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
//			}
//		} while (_findnext(hFile, &fileinfo) == 0);
//		_findclose(hFile);
//	}
//}