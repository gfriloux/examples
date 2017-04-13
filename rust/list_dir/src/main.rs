extern crate glob;
extern crate regex;

use glob::glob;
use std::result::Result;
use regex::Regex;

fn main() {
   let re = Regex::new(".*(info|stat).json$").unwrap();

   for path in glob("test/*").unwrap().filter_map(Result::ok)
                             .skip_while(|x| x.ends_with("info.json"))
                             .skip_while(|x| x.ends_with("stat.json")) {

      println!("{}", path.display());
   }

//   for path in glob("test/*").unwrap().filter_map(Result::ok) {
//      if re.is_match(path.to_str().unwrap()) {
//         continue;
//      }
//      println!("{}", path.display());
//   }
}
