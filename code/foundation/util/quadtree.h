#pragma once
//------------------------------------------------------------------------------
/**
    @class QuadTree
    @ingroup Util

    A simple quad tree. QuadTree elements are template nodes and
    are inserted and removed from a quadtree by bounding box.

    (C) 2007 RadonLabs GmbH
*/
#include "util/fixedarray.h"
#include "math/bbox.h"

namespace Util
{
using namespace Math;
//------------------------------------------------------------------------------
template<class TYPE> class QuadTree
{
public:
    class Node;    

    /// constructor
    QuadTree();
    /// destructor
    ~QuadTree();
    /// initialize quad tree
    void Initialize();
    /// set surrounding bounding box of quadtree
    void SetBoundingBox(const Math::bbox& box);
    /// set the tree depth
    void SetDepth(uchar depth);
    /// get the tree depth
    uchar GetDepth() const;
    /// get the top level bounding box
    const Math::bbox& GetBoundingBox() const;
    /// insert element into quad tree
    void Insert(TYPE* elm, const Math::bbox& box);
    /// remove an element from the quad tree
    void Remove(TYPE* elm);
    /// compute number of nodes in a level, including its children
    int GetNumNodes(uchar level) const;
    /// compute linear chunk index from level, col and row
    int GetNodeIndex(uchar level, ushort col, ushort row) const;
    /// get pointer to node by index
    const Node& GetNodeByIndex(int i) const;

    /// node in quad tree
    class Node
    {
    public:
        /// constructor
        Node();
        /// destructor
        ~Node();
        /// recursively initialize the node
        void Initialize(QuadTree<TYPE>* tree, uchar _level, ushort _col, ushort _row);
        /// get the node's level
        char Level() const;
        /// get the node's column
        ushort Column() const;
        /// get the node's row
        ushort Row() const;
        /// compute the node's bounding box
        const Math::bbox& GetBoundingBox() const;
        /// recursively find the smallest child node which contains the bounding box
        Node* FindContainmentNode(const Math::bbox& box);        
        /// clear element ptr
        void ClearDataPtr();
        /// set element ptr
        void SetDataPtr(TYPE* elem);

    private:
        friend class QuadTree;

        Node* children[4];
        char level;
        ushort col;
        ushort row;
        Math::bbox box;
        TYPE* data;
    };

private:
    friend class Node;

    uchar treeDepth;
    Math::bbox boundingBox;                  // global bounding box
    Math::vector baseNodeSize;               // base chunk bounding box
    Util::FixedArray<Node> nodeArray;
};

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
QuadTree<TYPE>::Node::Node() :
    level(-1),
    col(0),
    row(0)
{
    int i;
    for (i = 0; i < 4; i++)
    {
        this->children[i] = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
QuadTree<TYPE>::Node::~Node()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> const typename QuadTree<TYPE>::Node&
QuadTree<TYPE>::GetNodeByIndex(int index) const
{
    return this->nodeArray[index];
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void 
QuadTree<TYPE>::SetBoundingBox(const Math::bbox& box)
{
    this->boundingBox = box;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> const Math::bbox&
QuadTree<TYPE>::Node::GetBoundingBox() const
{
    return this->box;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> char
QuadTree<TYPE>::Node::Level() const
{
    return this->level;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> ushort
QuadTree<TYPE>::Node::Column() const
{
    return this->col;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> ushort
QuadTree<TYPE>::Node::Row() const
{
    return this->row;
}

//------------------------------------------------------------------------------
/**
    Sets depth of quad tree.
*/
template<class TYPE> void 
QuadTree<TYPE>::SetDepth(uchar depth)
{
    this->treeDepth = depth;
}

//------------------------------------------------------------------------------
/**
    Returns depth of quad tree.
*/
template<class TYPE> uchar
QuadTree<TYPE>::GetDepth() const
{
    return this->treeDepth;
}

//------------------------------------------------------------------------------
/**
    Returns top level bounding box of quad tree.
*/
template<class TYPE> const Math::bbox&
QuadTree<TYPE>::GetBoundingBox() const
{
    return this->boundingBox;
}

//------------------------------------------------------------------------------
/**
    Computes number of nodes in a level, including its child nodes.
*/
template<class TYPE> int
QuadTree<TYPE>::GetNumNodes(uchar level) const
{
    return 0x55555555 & ((1 << level * 2) - 1);
}

//------------------------------------------------------------------------------
/**
    Computes a linear chunk index for a chunk address consisting of 
    level, col and row.
*/
template<class TYPE> int
QuadTree<TYPE>::GetNodeIndex(uchar level, ushort col, ushort row) const
{
    #if NEBULA3_BOUNDSCHECKS
    n_assert((col >= 0) && (col < (1 << level)));
    n_assert((row >= 0) && (row < (1 << level)));
    #endif
    return this->GetNumNodes(level) + (row << level) + col;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
QuadTree<TYPE>::Node::ClearDataPtr()
{
    this->data = 0;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
QuadTree<TYPE>::Node::SetDataPtr(TYPE* elem)
{
    #if NEBULA3_BOUNDSCHECKS
    n_assert(0 != elem);
    #endif
    this->data = elem;
}

//------------------------------------------------------------------------------
/**
    QuadTree constructor.
*/
template<class TYPE>
QuadTree<TYPE>::QuadTree() :
    treeDepth(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
    QuadTree destructor.
*/
template<class TYPE>
QuadTree<TYPE>::~QuadTree()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Initialize the quad tree.
*/
template<class TYPE> void
QuadTree<TYPE>::Initialize()
{
    #if NEBULA3_BOUNDSCHECKS    
    n_assert(this->treeDepth > 0);
    n_assert(float4::all(float4::greater(this->boundingBox.extents().abs(), float4(0,0,0,0))));
    #endif

    int baseDimension = 1 << (this->treeDepth - 1);
    this->baseNodeSize.set(this->boundingBox.size().x() / baseDimension,
                           this->boundingBox.size().y(),                                                                          
                           this->boundingBox.size().z() / baseDimension);

    int numNodes = this->GetNumNodes(this->treeDepth);
    this->nodeArray.SetSize(numNodes);
    this->nodeArray[0].Initialize(this, 0, 0, 0);

    // make sure all nodes have been initialized
    #if NEBULA3_BOUNDSCHECKS
    int i;
    int num = this->nodeArray.Size();
    for (i = 0; i < num; i++)
    {
        n_assert(this->nodeArray[i].Level() >= 0);
    }
    #endif
}

//------------------------------------------------------------------------------
/**
    This inserts an element into the quad tree.
*/
template<class TYPE> void
QuadTree<TYPE>::Insert(TYPE* elm, const Math::bbox& box)
{
    #if NEBULA3_BOUNDSCHECKS
    n_assert(elm);
    #endif

    // find the smallest node which completely encloses the element's bbox
    Node<TYPE>* node = this->nodeArray[0].FindContainmentNode(box);
    if (node)
    {
        node-> = elm;
    }
    else
    {
        // NOTE: technically, the node is outside of the world,
        // just attach it to the toplevel node
        this->nodeArray[0]->data = elm;
    }
}

//------------------------------------------------------------------------------
/**
    Removes an element from the quad tree.
*/
template<class TYPE> void
QuadTree<TYPE>::Remove(TYPE* elm)
{
    #if NEBULA3_BOUNDSCHECKS
    n_assert(elm);
    #endif
    elm->ClearDataPtr();
}

//------------------------------------------------------------------------------
/**
    Recursively initialize a quad tree node.
*/
template<class TYPE> void
QuadTree<TYPE>::Node::Initialize(QuadTree* tree, uchar _level, ushort _col, ushort _row)
{
    #if NEBULA3_BOUNDSCHECKS
    n_assert(tree);
    n_assert(this->level == -1);
    n_assert(_level >= 0);
    n_assert((_col >= 0) && (_col < (1 << _level)));
    n_assert((_row >= 0) && (_row < (1 << _level)));
    #endif

    // store address
    this->level = _level;
    this->col   = _col;
    this->row   = _row;

    // update my bounding box
    float levelFactor = float(1 << (tree->treeDepth - 1 - this->level));
    static vector center;
    static vector extent;
    const vector& baseSize = tree->baseNodeSize;
    const bbox& treeBox = tree->boundingBox;
    vector treeSize = treeBox.size();
    vector treeCenter = treeBox.center();

    center.set( treeCenter.x() + (((this->col + 0.5f) * levelFactor * baseSize.x()) - (treeSize.x() * 0.5f)),
                treeCenter.y(),
                treeCenter.z() + (((this->row + 0.5f) * levelFactor * baseSize.z()) - (treeSize.z() * 0.5f)));

    extent.set( levelFactor * baseSize.x() * 0.5f,
                treeBox.extents().y(),
                levelFactor * baseSize.z() * 0.5f );

    this->box.set(center, extent);

    // recurse into children
    uchar childLevel = this->level + 1;
    if (childLevel < tree->treeDepth)
    {
        ushort i;
        for (i = 0; i < 4; i++)
        {
            ushort childCol = 2 * this->col + (i & 1);
            ushort childRow = 2 * this->row + ((i & 2) >> 1);
            int childIndex = tree->GetNodeIndex(childLevel, childCol, childRow);
            this->children[i] = &(tree->nodeArray[childIndex]);
            this->children[i]->Initialize(tree, childLevel, childCol, childRow);
        }
    }
}

//------------------------------------------------------------------------------
/**
    This finds the smallest child node which completely contains the
    given bounding box. Calls itself recursively.
*/
template<class TYPE> typename QuadTree<TYPE>::Node*
QuadTree<TYPE>::Node::FindContainmentNode(const Math::bbox& checkBox)
{
    if (this->box.contains(checkBox))
    {
        // recurse into children
        if (this->children[0] != 0)
        {
            int i;
            for (i = 0; i < 4; i++)
            {
                Node* containNode = this->children[i]->FindContainmentNode(checkBox);
                if (containNode)
                {
                    return containNode;
                }
            }
        }

        // not contained in children, but still contained in this
        return this;
    }
    else
    {
        // not contained in this, break recursion
        return 0;
    }
}

} // namespace Util
//------------------------------------------------------------------------------
