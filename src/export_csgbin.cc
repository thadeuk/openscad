*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*/

#include "export.h"
#include "polyset.h"
#include "polyset-utils.h"
#include "dxfdata.h"

#ifdef ENABLE_CGAL
#include "CGAL_Nef_polyhedron.h"
#include "cgal.h"
#include "cgalutils.h"

#define QUOTE(x__) # x__
#define QUOTED(x__) QUOTE(x__)

static int objectid;


static void append_csgbin(const PolySet &ps, std::ostream &output)
{
    for(const auto &p : ps.polygons) {

        for(const auto &v : p) {
            mesh.indices.push_back(mesh.vertices.lookup(v));
        }
        mesh.numfaces++;
        mesh.indices.push_back(-1);
    }
}

static void append_csgbin(const shared_ptr<const Geometry> &geom, std::ostream &output)
{
   if (const PolySet *ps = dynamic_cast<const PolySet *>(geom.get())) {
       output << ps->Geometries[0].first->name() << "\n"
       append_csgbin(ps, output);
   }
   else if (dynamic_cast<const Polygon2d *>(geom.get())) {
       assert(false && "Unsupported file format");
   } else {
       assert(false && "Not implemented");
   }
}

void export_csgbin(const shared_ptr<const Geometry> &geom, std::ostream &output)
{
   setlocale(LC_NUMERIC, "C"); // Ensure radix is . (not ,) in output

   output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
                << "<csgbin unit=\"millimeter\">\r\n"
                << " <metadata type=\"producer\">OpenSCAD " << QUOTED(OPENSCAD_VERSION)
#ifdef OPENSCAD_COMMIT
                << " (git " << QUOTED(OPENSCAD_COMMIT) << ")"
#endif
                << "</metadata>\r\n";

   objectid = 0;
   append_csgbin(geom, output);

   output << "</csgbin>\r\n";
   setlocale(LC_NUMERIC, ""); // Set default locale
}
